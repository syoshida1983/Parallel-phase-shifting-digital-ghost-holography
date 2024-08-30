// SPI_server.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <winuser.h>
#include <boost/program_options.hpp>
#include <opencv2/opencv.hpp>
#include "mat.hpp"

static std::vector< int32_t > left;
static std::vector< int32_t > width;
static std::vector< int32_t > height;

/// <summary>
/// callback function for getting monitor information
/// </summary>
/// <param name="hMonitor"></param>
/// <param name="hdcMonitor"></param>
/// <param name="lprcMonitor"></param>
/// <param name="dwData"></param>
/// <returns></returns>
BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	MONITORINFOEX monitorInfo;
	monitorInfo.cbSize = sizeof(monitorInfo);
	GetMonitorInfo(hMonitor, &monitorInfo);

	/// leftmost logical coordinate of monitors
	left.push_back(monitorInfo.rcMonitor.left);

	DEVMODE devMode;
	devMode.dmSize = sizeof(DEVMODE);
	devMode.dmDriverExtra = sizeof(POINTL);
	devMode.dmFields = DM_POSITION;
	EnumDisplaySettings(monitorInfo.szDevice, ENUM_CURRENT_SETTINGS, &devMode);

	/// physical resolution of monitors
	width.push_back(devMode.dmPelsWidth);
	height.push_back(devMode.dmPelsHeight);
	
	return TRUE;
}

/// <summary>
/// getting monitor information
/// </summary>
void get_monitor_info()
{
	EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, 0);
}

int32_t main(int argc, char *argv[])
{
	using namespace std;
	using namespace boost::program_options;
	using namespace cv;

	options_description desc("options");
	desc.add_options()
		("help,h",									"produce help message")
		("monitor,m",		value< uint32_t >(),	"set monitor number")
		("power,p",			value< uint32_t >(),	"set pattern size in powers of 2")
		("factor,f",		value< uint32_t >(),	"set enlargement factor")
		("level,l",			value< uint32_t >(),	"set gray level of white pixel")
		("background,b",	value< uint32_t >(),	"set gray level of background");

	variables_map vm;
	store(parse_command_line(argc, argv, desc), vm);
	notify(vm);

	if (vm.count("help") || !vm.count("monitor") || !vm.count("power") || !vm.count("factor") || !vm.count("level") || !vm.count("background"))
	{
		std::cout << desc << endl;
		return 0;
	}

	int32_t monitorNo	= vm["monitor"]		.as< uint32_t >();
	int32_t powerOfTwo	= vm["power"]		.as< uint32_t >();
	int32_t factor		= vm["factor"]		.as< uint32_t >();
	int32_t grayLevel	= vm["level"]		.as< uint32_t >();
	int32_t background	= vm["background"]	.as< uint32_t >();

	///< order of Walsh matrix
	const int32_t N = 1 << powerOfTwo;

	/// getting monitor information
	get_monitor_info();

	/// monitor number check
	if (monitorNo >= ::left.size() || monitorNo < 0)
	{
		cerr << "Invalid monitor number." << endl;
		exit(-1);
	}

	/// size check
	if (N * factor > width[monitorNo] || N * factor > height[monitorNo])
	{
		cerr << "Invalid image size." << endl;
		exit(-1);
	}

	/// creating window
	const std::string window = "window";
	namedWindow(window, WINDOW_NORMAL);
	moveWindow(window, ::left[monitorNo], 0);
	setWindowProperty(window, WND_PROP_FULLSCREEN, WINDOW_FULLSCREEN);
	imshow(window, Mat(height[monitorNo], width[monitorNo], CV_8UC1, Scalar(0)));
	waitKey(1);

	/// creating named pipe
	HANDLE pipe = CreateNamedPipe(L"\\\\.\\pipe\\SPI", PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE | PIPE_WAIT, 1, 0, 0, 0, NULL);

	if (pipe == INVALID_HANDLE_VALUE)
	{
		cerr << "Could not create pipe." << endl;
		exit(-1);
	}

	if (!ConnectNamedPipe(pipe, NULL))
	{
		CloseHandle(pipe);
		cerr << "Could not connect pipe." << endl;
		exit(-1);
	}

	/// creating Walsh matrix
	const Mat W = Walsh< float >(powerOfTwo);

	/// creating look up table
	vector< int32_t > x(N * N);
	vector< int32_t > y(N * N);
	auto idx = 0;
	for (auto i = 1; i <= N; i++)
	{
		for (auto j = 1; j <= i; j++)
		{
			x[idx] = j - 1;
			y[idx] = i - j;
			idx++;
		}
	}
	for (auto i = N - 1; i > 0; i--)
	{
		for (auto j = N - i, k = 1; j < N; j++, k++)
		{
			x[idx] = j;
			y[idx] = N - k;
			idx++;
		}
	}

	DWORD bytes;
	int32_t rx;
	const char tx = 0x06;	///< ACK

	while (1)
	{
		if (ReadFile(pipe, &rx, sizeof(rx), &bytes, NULL))
		{
			if (rx == -1)
			{
				while (!WriteFile(pipe, &tx, 1, NULL, NULL));
				break;
			}
			else
			{
				/// creating orthogonal pattern
				const auto i = x[rx];
				const auto j = y[rx];
				const Mat H = (W.col(i) * W.row(j) + 1) / 2;

				/// creating display pattern
				Mat page;
				H.convertTo(page, CV_8UC1, grayLevel - background, background);
				resize(page, page, Size(H.cols * factor, H.rows * factor), factor, factor, INTER_NEAREST);
				Mat	disp(height[monitorNo], width[monitorNo], page.type(), Scalar::all(background));
				page.copyTo(disp(Rect((disp.cols - page.cols) / 2, (disp.rows - page.rows) / 2, page.cols, page.rows)));

				imshow(window, disp);
				waitKey(1);

				while (!WriteFile(pipe, &tx, 1, NULL, NULL));
			}
		}
	}

	FlushFileBuffers(pipe);
	DisconnectNamedPipe(pipe);
	CloseHandle(pipe);

	return 0;
}
