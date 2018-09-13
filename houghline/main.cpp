#pragma warning(disable: 4819)

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat src = imread("building.jpg", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	Mat src_edge, dst;
	Canny(src, src_edge, 50, 150);
	cvtColor(src_edge, dst, COLOR_GRAY2BGR);

	double start = double(getTickCount());

#if 1
	vector<Vec2f> lines;
	HoughLines(src_edge, lines, 1, CV_PI / 180, 250);

	double duration_ms = (double(getTickCount()) - start) * 1000 / getTickFrequency();
	cout << "It took " << duration_ms << " ms." << endl;

	for (size_t i = 0; i < lines.size(); i++) {
		float r = lines[i][0], t = lines[i][1];
		double cos_t = cos(t), sin_t = sin(t);
		double x0 = r*cos_t, y0 = r*sin_t;
		double alpha = 1000;

		Point pt1(cvRound(x0 + alpha*(-sin_t)), cvRound(y0 + alpha*cos_t));
		Point pt2(cvRound(x0 - alpha*(-sin_t)), cvRound(y0 - alpha*cos_t));
		line(dst, pt1, pt2, Scalar(0, 0, 255), 2, LINE_AA);
	}
#else
	vector<Vec4i> lines;
	HoughLinesP(src_edge, lines, 1, CV_PI / 180, 160, 50, 5);

	double duration_ms = (double(getTickCount()) - start) * 1000 / getTickFrequency();
	cout << "It took " << duration_ms << " ms." << endl;

	for (size_t i = 0; i < lines.size(); i++) {
		Vec4i l = lines[i];
		line(dst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 2, LINE_AA);
	}
#endif

	imshow("src", src);
	imshow("dst", dst);
	imshow("edge", src_edge);
	waitKey(0);
	return 0;
}