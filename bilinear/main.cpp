#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void resizeBilinear(const Mat& src, Mat& dst, Size size);

int main()
{
	Mat src = imread("camera.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	Mat dst;

#if 1
	resize(src, dst, Size(400, 200), INTER_LINEAR);
#else
	resizeBilinear(src, dst, Size(400, 200));
#endif

	imshow("src", src);
	imshow("dst", dst);

	waitKey(0);
	return 0;
}

void resizeBilinear(const Mat& src, Mat& dst, Size size)
{
	if (dst.empty() || dst.rows != size.height ||
		dst.cols != size.width || dst.type() != CV_8U) {
		dst.create(size.height, size.width, CV_8U);
	}

	int x1, y1, x2, y2;
	double rx, ry, p, q, value;
	double sx = static_cast<double>(src.cols - 1) / (dst.cols - 1);
	double sy = static_cast<double>(src.rows - 1) / (dst.rows - 1);

	for (int j = 0; j < dst.rows; j++) {
		for (int i = 0; i < dst.cols; i++) {
			rx = sx * i;
			ry = sy * j;

			x1 = cvFloor(rx);
			y1 = cvFloor(ry);

			x2 = x1 + 1; if (x2 == src.cols) x2 = src.cols - 1;
			y2 = y1 + 1; if (y2 == src.rows) y2 = src.rows - 1;

			p = rx - x1;
			q = ry - y1;

			value = (1. - p) * (1. - q) * src.at<uchar>(y1, x1)
				+ p * (1. - q) * src.at<uchar>(y1, x2)
				+ (1. - p) * q * src.at<uchar>(y2, x1)
				+ p * q * src.at<uchar>(y2, x2);

			dst.at<uchar>(j, i) = static_cast<uchar>(value+.5);
		}
	}
}
