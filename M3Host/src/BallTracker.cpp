#include "BallTracker.h"

using namespace cv;

BallTracker::BallTracker() {
}

void BallTracker::init(const char *configfilename){
	return;
}

int BallTracker::SquareDistance(Point2i &First, Point2i &Second){
	return (First.x - Second.x)*(First.x - Second.x) + (First.y - Second.y)*(First.y - Second.y);
}

int BallTracker::FindClosestVisibleBall(Point2i &NewBall,bool UsePredict)
{
	int ClosestBall=0;
	if (Balls.empty()) return -1;
	else
	{
		for (int i = 1; i < Balls.size() ; i++)
		{
			if( Balls[i].GetVisible())
				if (SquareDistance(NewBall, Balls[i].GetPosition())<SquareDistance(NewBall, Balls[ClosestBall].GetPosition()))
					ClosestBall = i;
		}
		if (SquareDistance(NewBall, Balls[ClosestBall].GetPosition()) > 2500)
			return -1;
	}
	return ClosestBall;
}

void BallTracker::FindBallContoursUsingHSV(Mat& img, Scalar Low, Scalar High)
{
	Mat imgThresh, imgHSV;
	cvtColor(img, imgHSV, CV_BGR2HSV);
	inRange(imgHSV, Low, High, imgThresh);
	findContours(imgThresh, Contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
}

void BallTracker::MatchContoursWithBalls(Mat& fortesting, bool UsePredict)
{
	for (unsigned int i = 0; i < Balls.size(); i++)
	{
		Balls[i].IncrementLastSeen();
	}

	for (unsigned int i = 0; i < Contours.size(); i++)
	{
		if (Contours[i].size()>30)
		{
			float radius;
			Point2f center;
			minEnclosingCircle(Contours[i], center, radius);
			if (FindClosestVisibleBall((Point2i)center) == -1) // új labda
			{
				Ball NewBall((int)center.x, (int)center.y, (int)radius);
				Balls.push_back(NewBall);
			}
			else
			{
				Balls[FindClosestVisibleBall((Point2i)center)].UpdateData(center.x,center.y,radius);
			}
			circle(fortesting, center, radius, Scalar(255, 0, 0)); // for testing
		}
	}
}

void BallTracker::DrawVisibleBallRoutes(Mat &img)
{
	for (int i = 0; i < Balls.size(); i++)
	{
		if (Balls[i].GetLastSeen()>5) Balls[i].SetVisible(false);
		if (Balls[i].GetVisible()) Balls[i].DrawLine(img);
	}
}

void BallTracker::processFrame(Mat& img){
	/*HSV:0-180, 0-97, 201-256 */
	FindBallContoursUsingHSV(img, Scalar(0, 0, 201), Scalar(180, 97, 256));
	MatchContoursWithBalls(img);
	DrawVisibleBallRoutes(img);
	imshow("pic", img);
}



BallTracker::~BallTracker() {
}