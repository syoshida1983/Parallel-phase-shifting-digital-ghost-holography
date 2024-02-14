#include "pch.h"
#include "SPI_client.h"

HANDLE pipe;

int32_t open()
{
	pipe = CreateFile(L"\\\\.\\pipe\\SPI", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (pipe == INVALID_HANDLE_VALUE)
	{
		return -1;
	}

	return 0;
}

void close()
{
	CloseHandle(pipe);
}

void write(const int32_t i)
{
	std::array< char, 256 > rx;
	while (!WriteFile(pipe, &i, sizeof(i), NULL, NULL));
	while (!ReadFile(pipe, rx.data(), sizeof(rx), NULL, NULL));
}
