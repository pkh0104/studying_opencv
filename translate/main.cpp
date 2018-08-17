#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

Mat translate(const Mat& src, double x, double y);

int main()
{
	Mat src = imread("lenna.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	Mat dst = translate(src, 50., 70.);

	imshow("src", src);
	imshow("dst", dst);

	waitKey(0);
	return 0;
}

Mat translate(const Mat& src, double x, double y)
{
	CV_Assert(!src.empty());

	Mat dst(src.rows * 2, src.cols * 2, src.type());
	Mat t(2, 3, CV_64F);
    t.at<double>(0, 0) = 1;
    t.at<double>(0, 1) = 0;
    t.at<double>(0, 2) = x;
    t.at<double>(1, 0) = 0;
    t.at<double>(1, 1) = 1;
    t.at<double>(1, 2) = y;
    cout << t << endl;
	warpAffine(src, dst, t, Size(src.cols, src.rows));
	return dst;
}
