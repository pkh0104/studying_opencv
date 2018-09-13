
#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdio.h>

#include "utils.h"


using namespace std;
using namespace cv;

//ROI
int L_ROI_x = 180;
int L_ROI_y = 290;
int L_ROI_w = 150;
int L_ROI_h = 90;

int R_ROI_x = L_ROI_x + L_ROI_w;
int R_ROI_y = L_ROI_y;
int R_ROI_w = L_ROI_w;
int R_ROI_h = L_ROI_h;

cv::Rect rLeft, rRight;

//���� ���ϴ� ����
int UpHorizLn = 40;

//���� ������ ���� ����
cv::Point LRoiPt = cv::Point(L_ROI_x, L_ROI_y), RRoiPt = cv::Point(R_ROI_x, R_ROI_y);
cv::Point HeadingPt = cv::Point(320, UpHorizLn + LRoiPt.y); // ������ ������. roi�� �����°��� ���ذ��� ���� ���� �ָ� ���� ���ؼ����� �����ٶ󺸰� �ִ� ��. �̰��� ���� ī�޶� �̸��극�̼��� ���� �ٶ󺸴� �������� ��ƾ� �Ѵ�.

int main() {
	// ���� ���Ϳ� ������͸� ���� ���� ���� ����
	float alpha = 0.5;  // ��� ���� ����ġ (0 ~ 0.1)
	cv::Point2f LLnMovAvg = Point2f(0, 0); // ���� ���� �ʱ� ��.
	cv::Point2f RLnMovAvg = Point2f(0, 0);//  ������ ���� �ʱ� ��.

										  //������ �ӵ� ����� ���� ����
	double tick_st, tick_end, time_elap;

	// open ������ ���� ����
	VideoCapture capture;
	capture.open("test1.avi");

	//image format
	Mat ori_frame;
	Mat resize_frame;
	Mat frame;
	Mat ROI_Lframe, ROI_Rframe, ROI_Lframe_edge, ROI_Rframe_edge;

	char keyin = 0;

	// LDW 
	while (keyin != 27) {  // esc Ű���� �� �ƽ�Ű �ڵ� ���� 27��

						   // FRAME ���ǵ带 �˱� ���� TICK �߻�.
		tick_st = cvGetTickCount();

		//camera capture �� ���������� ����
		capture >> ori_frame;
		if (ori_frame.empty()) { break; }

		//resize
		cv::Size process_size = cv::Size(640, 480);
		resize(ori_frame, resize_frame, process_size, INTER_NEAREST);

		// gray scale
		cv::cvtColor(resize_frame, frame, COLOR_BGR2YUV);

		Mat chan[3];
		split(resize_frame, chan);
		frame = chan[0];

		////////////// ���� ���� ����
		//�����ӿ��� ���� ���� roi �����ϱ�
		rLeft = cv::Rect(L_ROI_x, L_ROI_y, L_ROI_w, L_ROI_h);   //cvRct(���� x��ǥ, ���� y��ǥ, xy��ǥ���� ����,xy��ǥ���� ����)
		frame(rLeft).copyTo(ROI_Lframe);
		cv::rectangle(resize_frame, cv::Point(L_ROI_x, L_ROI_y), cv::Point(L_ROI_x + L_ROI_w, L_ROI_y + L_ROI_h), CV_RGB(0, 0, 255), 1, 8, 0);//cvpoint(x,y) cvpoint�� ���� ���� ������ �Ʒ� ��ǥ�� �־� �簢�� �׸���.

																																			  // ���� �����ӿ��� Edge ����
		cv::Canny(ROI_Lframe, ROI_Lframe_edge, 100, 150, 3);

		// Edge ���Ⱚ�� ����� edge_frame���� ���� Ʈ������ �ϱ����� ���� ���� Ʈ���� ���� ����
		std::vector<cv::Vec2f> Llines;

		//���� Ʈ���� ������ edge�� ����� edge_fram���� ����Ʈ���� ��ȯ�� ���� ����
		cv::HoughLines(ROI_Lframe_edge, Llines, 1, CV_PI / 180, 30);

		// ������ ���� ���� �� 30��~60�� ������ ������ ����
		cv::Point2f LLnPt;
		LLnPt = filtHoughlines(0, Llines, 25, resize_frame(rLeft)); //filtHoughlines(0�̸� ���� 1�̸� ������, ���� ����, ANGLE , ROI���� ������)

																	// ��� ���ͷ� �������� ������ �ϳ��� �������� ����
		if (fabs(LLnPt.x) <= 200) // ������ ���������� �Ÿ��� 200���� ������ ������ �ƴ� ����. �׷��Ƿ� FRAME�� ũ�⳪ ROI�� ũ�⿡ ���� �Ÿ� ���� ���� �� ��
		{
			LLnMovAvg.x = alpha*LLnPt.x + (1 - alpha)*LLnMovAvg.x;
			LLnMovAvg.y = alpha*LLnPt.y + (1 - alpha)*LLnMovAvg.y;
		}

		// utils2 ������Ͽ��� ������ ��ǥ��� �������� ��ǥ�� ��ġ ���Ѽ� ���� ǥ������
		drawPolarline(LLnMovAvg, cv::Point(L_ROI_x, L_ROI_y), resize_frame);



		//////////// ������ ���� ����

		//�����ӿ��� ������ ���� roi �����ϱ�.
		rRight = cv::Rect(R_ROI_x, R_ROI_y, R_ROI_w, R_ROI_h);   //cvRct(���� x��ǥ, ���� y��ǥ, xy��ǥ���� ����,xy��ǥ���� ����)
		frame(rRight).copyTo(ROI_Rframe);
		cv::rectangle(resize_frame, cv::Point(R_ROI_x, R_ROI_y), cv::Point(R_ROI_x + R_ROI_w, R_ROI_y + R_ROI_h), CV_RGB(0, 0, 255), 1, 8, 0); //cvpoint(x,y) cvpoint�� ���� ���� ������ �Ʒ� ��ǥ�� �־� �簢�� �׸���.

																																			   // ���� �����ӿ��� Edge ����
		cv::Canny(ROI_Rframe, ROI_Rframe_edge, 100, 150, 3);

		// Edge ���Ⱚ�� ����� edge_frame���� ���� Ʈ������ �ϱ����� ���� ���� Ʈ���� ���� ����
		std::vector<cv::Vec2f> Rlines;

		//���� Ʈ���� ������ edge�� ����� edge_fram���� ����Ʈ���� ��ȯ�� ���� ����
		cv::HoughLines(ROI_Rframe_edge, Rlines, 1, CV_PI / 180, 30);

		// ������ ���� ���� �� 30��~60�� ������ ������ ����
		cv::Point2f RLnPt;
		RLnPt = filtHoughlines(1, Rlines, 25, resize_frame(rRight));  //filtHoughlines(0�̸� ���� 1�̸� ������, ���� ����, ANGLE , ROI���� ������)

																	  // ��� ���ͷ� �������� ������ �ϳ��� �������� ����
		if (fabs(RLnPt.x) <= 200)
		{
			RLnMovAvg.x = alpha*RLnPt.x + (1 - alpha)*RLnMovAvg.x;
			RLnMovAvg.y = alpha*RLnPt.y + (1 - alpha)*RLnMovAvg.y;
		}
		// utils2 ������Ͽ��� ������ ��ǥ��� �������� ��ǥ�� ��ġ ���Ѽ� ���� ǥ������
		drawPolarline(RLnMovAvg, cv::Point(R_ROI_x, R_ROI_y), resize_frame);

		// �˰��� �� TIC �߻� �� ����
		tick_end = cvGetTickCount();
		// ���۰� �P�� tic�� ������ �߻� ���� �ӵ� ��� Calculate elapsed time for image processing
		time_elap = (tick_end - tick_st) / cvGetTickFrequency();


		//���� �Է�
		char tmpStr[100];
		sprintf(tmpStr, "Processing Time: %10.2f ms", time_elap / 1000);
		putText(resize_frame, tmpStr, cv::Point(20, 20), FONT_HERSHEY_SIMPLEX, 0.7, CV_RGB(0, 0, 255), 2);

		// ������ �߾� �� ã��.
		cv::Point UpLfLnPt = add_i(getIntersect(polar2cart(LLnMovAvg), (-1 / tanf(LLnMovAvg.y)), cvPoint(0, UpHorizLn), 0), LRoiPt);
		cv::Point UpRtLnPt = add_i(getIntersect(polar2cart(RLnMovAvg), (-1 / tanf(RLnMovAvg.y)), cvPoint(0, UpHorizLn), 0), RRoiPt);
		cv::circle(resize_frame, UpLfLnPt, 2, CV_RGB(0, 255, 0), 3, 8, 0);
		cv::circle(resize_frame, UpRtLnPt, 2, CV_RGB(0, 255, 0), 3, 8, 0);
		cv::line(resize_frame, UpLfLnPt, UpRtLnPt, CV_RGB(255, 0, 0), 1, 8, 0);

		// �߾����� �̾� �߾� �� �߱�
		cv::Point UpCenterPt = cv::Point((UpLfLnPt.x + UpRtLnPt.x) / 2, UpHorizLn + L_ROI_y);
		cv::circle(resize_frame, UpCenterPt, 2, CV_RGB(0, 255, 0), 3, 8, 0);

		// �۷ι� �߾��� ǥ��
		cv::circle(resize_frame, HeadingPt, 3, CV_RGB(255, 0, 0), 3, 8, 0);

		// �۷ι� �߾����� ���� �߾������� ���̷� ���� ��Ż ���
		float warning = (float)(UpCenterPt.x - HeadingPt.x);
		if (warning >6) warning_drawPolarline(LLnMovAvg, cvPoint(L_ROI_x, L_ROI_y), resize_frame);
		else if (warning < -6) warning_drawPolarline(RLnMovAvg, cvPoint(R_ROI_x, R_ROI_y), resize_frame);

		// ����� �����Ӱ��� �����ֱ�.
		imshow("original_video", resize_frame);
		imshow("edge_Lframe", ROI_Lframe_edge);
		imshow("edge_Rframe", ROI_Rframe_edge);
		imshow("LINE_Lframe", ROI_Lframe);
		imshow("LINE_Rframe", ROI_Rframe);
		waitKey(1);

		// �����ִ� �������� WINDOW â���� ������ ��ǥ���� �־� ���ġ �ϱ�
		cvMoveWindow("original_video", 50, 0);
		cvMoveWindow("LINE_Rframe", 50 + resize_frame.size().width + ROI_Rframe.size().width, 0);
		cvMoveWindow("LINE_Lframe", 50 + resize_frame.size().width, 0);
		cvMoveWindow("edge_Rframe", 50 + resize_frame.size().width + ROI_Rframe.size().width, ROI_Rframe.size().height + 30);
		cvMoveWindow("edge_Lframe", 50 + resize_frame.size().width, ROI_Rframe.size().height + 30);

		//ī���� �Է� 30ms ��ٸ���
		keyin = cvWaitKey(30);

	}
	// ĸ�ĸ� ����.
	capture.release();
	return 0;
}


