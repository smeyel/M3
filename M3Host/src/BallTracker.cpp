#include "BallTracker.h"

using namespace cv;

BallTracker::BallTracker() {
}

void BallTracker::init(const char *configfilename){
	return;
}

void BallTracker::processFrame(Mat& img){
	/*HSV:0-180, 0-97, 201-256 */
	Mat imgHSV,imgThresh;
	cvtColor(img, imgHSV, CV_BGR2HSV); //Change the color format from BGR to HSV
	inRange(imgHSV, Scalar(0, 0, 201), Scalar(180, 97, 256), imgThresh);
	//kontúr keresés
	vector<vector<Point>> kontur;
	findContours(imgThresh, kontur, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	for (int i = 0; i < kontur.size(); i++)
	{
		if (kontur[i].size()>40)
		{
			float radius;
			Point2f center;
			minEnclosingCircle(kontur[i], center, radius);
			circle(img, (Point2i)(center), (int)radius, Scalar(255, 0, 0));
		}
	}
	imshow("pic", img);
}



BallTracker::~BallTracker() {
}