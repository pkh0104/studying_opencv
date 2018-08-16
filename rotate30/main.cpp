#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

Mat rotate(const Mat& src, double degree);

int main()
{
	Mat src = imread("lenna.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	Mat dst = rotate(src, 30.);

	imshow("src", src);
	imshow("dst", dst);

	waitKey(0);
	return 0;
}

Mat rotate(const Mat& src, double degree)
{
	CV_Assert(!src.empty());

	Mat dst(src.rows * 2, src.cols * 2, src.type());
	Point2f pt(src.cols / 2.f, src.rows / 2.f);
	Mat r = getRotationMatrix2D(pt, degree, 1.0);
	cout << " rotaion matrix : " << r << endl;
	cout << " rotaion matrix : " << r.at<double>(0.0) << endl;
	warpAffine(src, dst, r, Size(src.cols, src.rows));
	return dst;
}
