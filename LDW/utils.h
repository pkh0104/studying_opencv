#pragma once

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

float rad2deg(float rad) { return float(rad * 180 / CV_PI); }
float deg2rad(float deg) { return float(deg * CV_PI / 180); }

cv::Point polar2cart(cv::Point Pt)
{
	int X = (int)(Pt.x * cos(Pt.y));
	int	Y = (int)(Pt.x * sin(Pt.y));

	cv::Point res = cv::Point(X, Y);
	return res;
}

cv::Point add_i(cv::Point b, cv::Point a) { return cv::Point(b.x + a.x, b.y + a.y); }

void drawHoughlines(std::vector<cv::Vec2f> lines, cv::Mat frame)
{
	for (size_t i = 0; i < lines.size(); i++)
	{

		float rho = lines[i][0];
		float theta = lines[i][1];

		CvPoint pt1, pt2;

		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));
		cv::line(frame, pt1, pt2, CV_RGB(0, 255, 0), 1, CV_AA, 0);
	}
}

cv::Point2f filtHoughlines(bool isRight, std::vector<cv::Vec2f> lines, int ang, cv::Mat frame)
{
	CvPoint pt1, pt2;
	int max_ang = 90, cnt = 0;
	float tmpTheta = 0, sumTheta = 0, sumRho = 0, sumDeviation = 0;

	for (size_t i = 0; i < lines.size(); i++)
	{
		float rho = lines[i][0];
		float theta = lines[i][1];

		if (isRight == true)
		{
			tmpTheta = CV_PI - theta;
		}
		else
		{
			tmpTheta = theta;
		}

		if (tmpTheta < deg2rad(max_ang - ang) && tmpTheta > deg2rad(ang))
		{
			//accurate theta & rho to calc average
			sumTheta += theta;
			sumRho += rho;
			cnt++;

			// Draw Hough lines
			double a = cos(theta), b = sin(theta);
			double x0 = a*rho, y0 = b*rho;
			pt1.x = cvRound(x0 + 1000 * (-b));
			pt1.y = cvRound(y0 + 1000 * (a));
			pt2.x = cvRound(x0 - 1000 * (-b));
			pt2.y = cvRound(y0 - 1000 * (a));
			cv::line(frame, pt1, pt2, CV_RGB(0, 255, 0), 1, CV_AA, 0);
		}
	}

	float avgRho = sumRho / cnt;
	float avgTheta = sumTheta / cnt;
	cv::Point2f avgPt = cv::Point2f(avgRho, avgTheta);
	return avgPt;
}

void drawPolarline(cv::Point2f pt, cv::Point offset, cv::Mat frame)
{

	float rho = pt.x;
	float theta = pt.y;

	cv::Point pt1, pt2;

	double a = cos(theta), b = sin(theta);
	double x0 = a*rho, y0 = b*rho;
	pt1.x = cvRound(x0 + 1000 * (-b));
	pt1.y = cvRound(y0 + 1000 * (a));
	pt2.x = cvRound(x0 - 1000 * (-b));
	pt2.y = cvRound(y0 - 1000 * (a));
	cv::line(frame, add_i(pt1, offset), add_i(pt2, offset), CV_RGB(0, 255, 0), 1, CV_AA, 0);
}

void warning_drawPolarline(cv::Point2f pt, cv::Point offset, cv::Mat frame)
{

	float rho = pt.x;
	float theta = pt.y;

	cv::Point pt1, pt2;

	double a = cos(theta), b = sin(theta);
	double x0 = a*rho, y0 = b*rho;
	pt1.x = cvRound(x0 + 1000 * (-b));
	pt1.y = cvRound(y0 + 1000 * (a));
	pt2.x = cvRound(x0 - 1000 * (-b));
	pt2.y = cvRound(y0 - 1000 * (a));
	cv::line(frame, add_i(pt1, offset), add_i(pt2, offset), CV_RGB(255, 0, 0), 1, CV_AA, 0);
	//CvPoint right_one_point = add_i(cvPoint(300, 150), polar2cart(RLnMovAvg));
	//cvCircle(frame, add_i(pt2, offset), 0.5, CV_RGB(255, 0, 0), 3, 8, 0);
}

cv::Point getIntersect(cv::Point pt1, float slope1, cv::Point pt2, float slope2)
{
	cv::Point intersect_pt(0, 0);
	if (abs(slope1 - slope2) == 0) return intersect_pt;

	// x = (m1x1 - m2x2 + y2 - y1) / (m1 - m2)
	intersect_pt.x = ((slope1 * pt1.x) - (slope2 * pt2.x) + pt2.y - pt1.y) / (slope1 - slope2);
	// y = m1(x - x1) + y1
	intersect_pt.y = slope1 * (intersect_pt.x - pt1.x) + pt1.y;

	return intersect_pt;
}
