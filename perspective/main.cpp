#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

Mat src, dst;
const String SRC_WND = "src";
Point2f srcQuad[4], dstQuad[4];

void on_mouse(int event, int x, int y, int flags, void*)
{
	static int cnt = 0;

	if (event == EVENT_LBUTTONDOWN) {
		srcQuad[cnt++] = Point2f(x, y);

		circle(src, Point(x, y), 7, Scalar(0, 0, 255), -1);
		imshow(SRC_WND, src);

		if (cnt == 4) {
			int w = 500, h = 150;
			dst.create(Size(w, h), CV_8SC3);

			dstQuad[0] = Point2f(0, 0);
			dstQuad[1] = Point2f(w, 0);
			dstQuad[2] = Point2f(w, h);
			dstQuad[3] = Point2f(0, h);

			for (int i = 0; i < 4; i++) {
				cout << i << ": " << srcQuad[i] << " => " << dstQuad[i] << endl;
			}

			Mat pers = getPerspectiveTransform(srcQuad, dstQuad);
			warpPerspective(src, dst, pers, Size(w, h));

			imshow("dst", dst);
		}
	}
	else if (event == EVENT_LBUTTONUP) {
		if (cnt == 4) {
			setMouseCallback(SRC_WND, 0);  //뒤에 0으로 인해 콜백함수를 해제한다. 
		}
	}
}

int main()
{
	src = imread("direction.jpeg");

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return -1;
	}

	namedWindow(SRC_WND);
	imshow(SRC_WND, src);

	setMouseCallback(SRC_WND, on_mouse);

	waitKey(0);
	return 0;
}
