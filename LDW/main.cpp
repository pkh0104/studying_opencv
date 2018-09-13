
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

//교점 구하는 변수
int UpHorizLn = 40;

//교점 교정을 위한 변수
cv::Point LRoiPt = cv::Point(L_ROI_x, L_ROI_y), RRoiPt = cv::Point(R_ROI_x, R_ROI_y);
cv::Point HeadingPt = cv::Point(320, UpHorizLn + LRoiPt.y); // 영상의 빨간점. roi의 오프셋값과 기준값을 더해 내가 멀리 보는 기준선에서 내가바라보고 있는 점. 이것은 원래 카메라 켈리브레이션을 통해 바라보는 기준점을 잡아야 한다.

int main() {
	// 허프 필터와 평균필터를 쓰기 위한 변수 선언
	float alpha = 0.5;  // 평균 필터 가중치 (0 ~ 0.1)
	cv::Point2f LLnMovAvg = Point2f(0, 0); // 왼쪽 차선 초기 값.
	cv::Point2f RLnMovAvg = Point2f(0, 0);//  오른쪽 차선 초기 값.

										  //프레임 속도 계산을 위한 변수
	double tick_st, tick_end, time_elap;

	// open 동영상 파일 오픈
	VideoCapture capture;
	capture.open("test1.avi");

	//image format
	Mat ori_frame;
	Mat resize_frame;
	Mat frame;
	Mat ROI_Lframe, ROI_Rframe, ROI_Lframe_edge, ROI_Rframe_edge;

	char keyin = 0;

	// LDW 
	while (keyin != 27) {  // esc 키보드 값 아스키 코드 값이 27임

						   // FRAME 스피드를 알기 위해 TICK 발생.
		tick_st = cvGetTickCount();

		//camera capture 값 프레임으로 저장
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

		////////////// 왼쪽 차선 감지
		//프레임에서 왼쪽 차선 roi 설정하기
		rLeft = cv::Rect(L_ROI_x, L_ROI_y, L_ROI_w, L_ROI_h);   //cvRct(시작 x좌표, 시작 y좌표, xy자표에서 넓이,xy자표에서 높이)
		frame(rLeft).copyTo(ROI_Lframe);
		cv::rectangle(resize_frame, cv::Point(L_ROI_x, L_ROI_y), cv::Point(L_ROI_x + L_ROI_w, L_ROI_y + L_ROI_h), CV_RGB(0, 0, 255), 1, 8, 0);//cvpoint(x,y) cvpoint로 왼쪽 위와 오른쪽 아래 좌표를 주어 사각형 그리기.

																																			  // 영상 프레임에서 Edge 추출
		cv::Canny(ROI_Lframe, ROI_Lframe_edge, 100, 150, 3);

		// Edge 추출값을 저장된 edge_frame에서 허프 트렌스를 하기위해 먼저 허프 트렌스 변수 선언
		std::vector<cv::Vec2f> Llines;

		//허프 트렌스 변수에 edge가 추출된 edge_fram에서 허프트렌스 변환된 값을 저장
		cv::HoughLines(ROI_Lframe_edge, Llines, 1, CV_PI / 180, 30);

		// 허프로 뽑은 직선 중 30도~60도 기울기의 직선만 감지
		cv::Point2f LLnPt;
		LLnPt = filtHoughlines(0, Llines, 25, resize_frame(rLeft)); //filtHoughlines(0이면 왼쪽 1이면 오른쪽, 허프 직선, ANGLE , ROI범위 프레임)

																	// 평균 필터로 여러개의 직선을 하나의 직선값을 구함
		if (fabs(LLnPt.x) <= 200) // 점에서 직선까지의 거리가 200보다 작으면 차선이 아닌 직선. 그러므로 FRAME의 크기나 ROI의 크기에 따라 거리 값도 조절 할 것
		{
			LLnMovAvg.x = alpha*LLnPt.x + (1 - alpha)*LLnMovAvg.x;
			LLnMovAvg.y = alpha*LLnPt.y + (1 - alpha)*LLnMovAvg.y;
		}

		// utils2 헤더파일에는 영상의 좌표계와 프레임의 좌표를 일치 시켜서 영상에 표시해줌
		drawPolarline(LLnMovAvg, cv::Point(L_ROI_x, L_ROI_y), resize_frame);



		//////////// 오른쪽 차선 감지

		//프레임에서 오른쪽 차선 roi 설정하기.
		rRight = cv::Rect(R_ROI_x, R_ROI_y, R_ROI_w, R_ROI_h);   //cvRct(시작 x좌표, 시작 y좌표, xy자표에서 넓이,xy자표에서 높이)
		frame(rRight).copyTo(ROI_Rframe);
		cv::rectangle(resize_frame, cv::Point(R_ROI_x, R_ROI_y), cv::Point(R_ROI_x + R_ROI_w, R_ROI_y + R_ROI_h), CV_RGB(0, 0, 255), 1, 8, 0); //cvpoint(x,y) cvpoint로 왼쪽 위와 오른쪽 아래 좌표를 주어 사각형 그리기.

																																			   // 영상 프레임에서 Edge 추출
		cv::Canny(ROI_Rframe, ROI_Rframe_edge, 100, 150, 3);

		// Edge 추출값을 저장된 edge_frame에서 허프 트렌스를 하기위해 먼저 허프 트렌스 변수 선언
		std::vector<cv::Vec2f> Rlines;

		//허프 트렌스 변수에 edge가 추출된 edge_fram에서 허프트렌스 변환된 값을 저장
		cv::HoughLines(ROI_Rframe_edge, Rlines, 1, CV_PI / 180, 30);

		// 허프로 뽑은 직선 중 30도~60도 기울기의 직선만 감지
		cv::Point2f RLnPt;
		RLnPt = filtHoughlines(1, Rlines, 25, resize_frame(rRight));  //filtHoughlines(0이면 왼쪽 1이면 오른쪽, 허프 직선, ANGLE , ROI범위 프레임)

																	  // 평균 필터로 여러개의 직선을 하나의 직선값을 구함
		if (fabs(RLnPt.x) <= 200)
		{
			RLnMovAvg.x = alpha*RLnPt.x + (1 - alpha)*RLnMovAvg.x;
			RLnMovAvg.y = alpha*RLnPt.y + (1 - alpha)*RLnMovAvg.y;
		}
		// utils2 헤더파일에는 영상의 좌표계와 프레임의 좌표를 일치 시켜서 영상에 표시해줌
		drawPolarline(RLnMovAvg, cv::Point(R_ROI_x, R_ROI_y), resize_frame);

		// 알고리즘 후 TIC 발생 값 저장
		tick_end = cvGetTickCount();
		// 시작과 긑의 tic의 값으로 발생 연산 속도 계산 Calculate elapsed time for image processing
		time_elap = (tick_end - tick_st) / cvGetTickFrequency();


		//글자 입력
		char tmpStr[100];
		sprintf(tmpStr, "Processing Time: %10.2f ms", time_elap / 1000);
		putText(resize_frame, tmpStr, cv::Point(20, 20), FONT_HERSHEY_SIMPLEX, 0.7, CV_RGB(0, 0, 255), 2);

		// 차선의 중앙 점 찾기.
		cv::Point UpLfLnPt = add_i(getIntersect(polar2cart(LLnMovAvg), (-1 / tanf(LLnMovAvg.y)), cvPoint(0, UpHorizLn), 0), LRoiPt);
		cv::Point UpRtLnPt = add_i(getIntersect(polar2cart(RLnMovAvg), (-1 / tanf(RLnMovAvg.y)), cvPoint(0, UpHorizLn), 0), RRoiPt);
		cv::circle(resize_frame, UpLfLnPt, 2, CV_RGB(0, 255, 0), 3, 8, 0);
		cv::circle(resize_frame, UpRtLnPt, 2, CV_RGB(0, 255, 0), 3, 8, 0);
		cv::line(resize_frame, UpLfLnPt, UpRtLnPt, CV_RGB(255, 0, 0), 1, 8, 0);

		// 중앙점을 이어 중앙 선 긋기
		cv::Point UpCenterPt = cv::Point((UpLfLnPt.x + UpRtLnPt.x) / 2, UpHorizLn + L_ROI_y);
		cv::circle(resize_frame, UpCenterPt, 2, CV_RGB(0, 255, 0), 3, 8, 0);

		// 글로벌 중앙점 표기
		cv::circle(resize_frame, HeadingPt, 3, CV_RGB(255, 0, 0), 3, 8, 0);

		// 글로벌 중앙점과 실제 중앙점과의 차이로 차선 이탈 경고
		float warning = (float)(UpCenterPt.x - HeadingPt.x);
		if (warning >6) warning_drawPolarline(LLnMovAvg, cvPoint(L_ROI_x, L_ROI_y), resize_frame);
		else if (warning < -6) warning_drawPolarline(RLnMovAvg, cvPoint(R_ROI_x, R_ROI_y), resize_frame);

		// 저장된 프레임값을 보여주기.
		imshow("original_video", resize_frame);
		imshow("edge_Lframe", ROI_Lframe_edge);
		imshow("edge_Rframe", ROI_Rframe_edge);
		imshow("LINE_Lframe", ROI_Lframe);
		imshow("LINE_Rframe", ROI_Rframe);
		waitKey(1);

		// 보여주는 프레임을 WINDOW 창에서 지정된 좌표값을 주어 재배치 하기
		cvMoveWindow("original_video", 50, 0);
		cvMoveWindow("LINE_Rframe", 50 + resize_frame.size().width + ROI_Rframe.size().width, 0);
		cvMoveWindow("LINE_Lframe", 50 + resize_frame.size().width, 0);
		cvMoveWindow("edge_Rframe", 50 + resize_frame.size().width + ROI_Rframe.size().width, ROI_Rframe.size().height + 30);
		cvMoveWindow("edge_Lframe", 50 + resize_frame.size().width, ROI_Rframe.size().height + 30);

		//카보드 입력 30ms 기다리기
		keyin = cvWaitKey(30);

	}
	// 캡쳐를 중지.
	capture.release();
	return 0;
}


