#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat src = imread("shrink.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

//	blur(src, src, Size(3, 3));

	namedWindow("src");
	imshow("src", src);

	Mat dst;
	int dsize = 128;
	namedWindow("dst");

	resize(src, dst, Size(dsize, dsize), 0, 0, INTER_NEAREST);
	cout << "INTER_NEAREST" << endl;
	imshow("dst", dst);
	waitKey(0);

	resize(src, dst, Size(dsize, dsize), 0, 0, INTER_LINEAR);
	cout << "INTER_LINEAR" << endl;
	imshow("dst", dst);
	waitKey(0);

	resize(src, dst, Size(dsize, dsize), 0, 0, INTER_CUBIC);
	cout << "INTER_CUBIC" << endl;
	imshow("dst", dst);
	waitKey(0);

	resize(src, dst, Size(dsize, dsize), 0, 0, INTER_LANCZOS4);
	cout << "INTER_LANCZOS4" << endl;
	imshow("dst", dst);
	waitKey(0);

	return 0;
}
