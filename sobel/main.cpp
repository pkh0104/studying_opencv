#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat src = imread("lenna.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	Mat dst1(src.rows, src.cols, src.type(), Scalar(0));
	for (int j = 1; j < dst1.rows - 1; j++) {
		for (int i = 1; i < dst1.cols - 1; i++) {
			int dx = -src.at<uchar>(j - 1, i - 1) - 2 * src.at<uchar>(j, i - 1) - src.at<uchar>(j + 1, i - 1)
				    + src.at<uchar>(j - 1, i + 1) + 2 * src.at<uchar>(j, i + 1) + src.at<uchar>(j + 1, i + 1);

			dst1.at<uchar>(j, i) = saturate_cast<uchar>(dx + 128);
		}
	}

	Mat dst2;
	Sobel(src, dst2, -1, 0, 1, 3, 1.0, 128);

	imshow("src", src);
	imshow("My Sobel y", dst1);
	imshow("OpenCV Sobel x", dst2);

	waitKey(0);
	return 0;
}
