#pragma once

/// <summary>
/// bit-reversal permutation
/// </summary>
/// <param name="a"></param>
/// <returns></returns>
uint32_t reverse_bits(const uint32_t a)
{
	uint32_t x = a;
	x = (((x & 0xaaaaaaaa) >> 1) | ((x & 0x55555555) << 1));
	x = (((x & 0xcccccccc) >> 2) | ((x & 0x33333333) << 2));
	x = (((x & 0xf0f0f0f0) >> 4) | ((x & 0x0f0f0f0f) << 4));
	x = (((x & 0xff00ff00) >> 8) | ((x & 0x00ff00ff) << 8));
	return((x >> 16) | (x << 16));
}

/// <summary>
/// Kronecker product
/// </summary>
/// <param name="A"></param>
/// <param name="B"></param>
/// <returns></returns>
template < class T >
cv::Mat kron(const cv::Mat_< T > &A, const cv::Mat_< T > &B)
{
	using namespace cv;

	CV_Assert(A.channels() == 1 && B.channels() == 1);

	Mat K(A.rows * B.rows, A.cols * B.cols, A.type(), Scalar(0));

	for (auto i = 0; i < A.rows; i++)
	{
		for (auto j = 0; j < A.cols; j++)
		{
			K(Range(i * B.rows, (i + 1) * B.rows), Range(j * B.cols, (j + 1) * B.cols)) = B.mul(A(i, j));
		}
	}

	return K;
}

/// <summary>
/// Hadamard matrix
/// </summary>
/// <param name="k">order of the Hadamard matrix (power of two)</param>
/// <returns>Hadamard matrix</returns>
template < class T >
cv::Mat Hadamard(const uint32_t k)
{
	using namespace cv;

	Mat H2 = (Mat_< T >(2, 2) << 1, 1, 1, -1);
	auto H = H2;

	for (auto i = 1u; i < k; i++)
	{
		H = kron< T >(H2, H);
	}

	return H;
}

/// <summary>
/// Walsh matrix
/// </summary>
/// <param name="k">order of the Hadamard matrix (power of two)</param>
/// <returns>Walsh matrix</returns>
template < class T >
cv::Mat Walsh(const uint32_t k)
{
	using namespace cv;

	const auto H = Hadamard< T >(k);
	Mat W(H.rows, H.cols, H.type());

	for (auto i = 0; i < H.rows; i++)
	{
		uint32_t j = reverse_bits(i) >> (31 - k);
		j = j ^ (j >> 1);
		j = (j << (32 - k)) >> (32 - k);
		H.row(j).copyTo(W.row(i));
	}

	return W;
}
