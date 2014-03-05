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

int BallTracker::FindClosestVisibleBall(Point2i &NewBall)
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
		if (SquareDistance(NewBall, Balls[ClosestBall].GetPosition()) > 10000)
			return -1;
	}
	return ClosestBall;
}

void BallTracker::processFrame(Mat& img){
	/*HSV:0-180, 0-97, 201-256 */
	cvtColor(img, imgHSV, CV_BGR2HSV); //Change the color format from BGR to HSV
	inRange(imgHSV, Scalar(0, 0, 201), Scalar(180, 97, 256), imgThresh);
	//kontúr keresés
	findContours(imgThresh.clone(), kontur, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	for (unsigned int i = 0; i < kontur.size(); i++)
	{
		if (kontur[i].size()>30)
		{
			float radius;
			Point2f center;
			minEnclosingCircle(kontur[i], center, radius);
			if (FindClosestVisibleBall((Point2i)center) == -1) // új labda
			{
				Ball NewBall((Point2i)center);
				Balls.push_back(NewBall);
			}
			else
			{
				Balls[FindClosestVisibleBall((Point2i)center)].UpdatePosition((Point2i)center);
			}
			circle(img, center, radius, Scalar(255, 0, 0));
		}
	}
	for (int i = 0; i < Balls.size(); i++)
	{
		Point2i Predicted = Balls[i].GetPosition();
		if (Balls[i].GetSize()>10 && (Predicted.x < 10 || Predicted.x>(img.cols - 10) || Predicted.y < 10 || Predicted.y >(img.rows - 10))) Balls[i].SetVisible(false);
		else if(Balls[i].GetSize()>1) Balls[i].DrawLine(img);
	}
	imshow("pic", img);
	imshow("th", imgThresh);
}



BallTracker::~BallTracker() {
}