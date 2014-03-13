#include "BallTracker.h"

using namespace cv;

BallTracker::BallTracker() {
}

void BallTracker::init(const char *configfilename){
	BallTrackerConfigManager config(configfilename);
	HSVlow = Scalar(config.Hlow, config.Slow, config.Vlow);
	HSVhigh = Scalar(config.Hhigh,config.Shigh,config.Vhigh);
	CollidedBallsAreaRate = (double)config.CollidedBallsAreaRateNum/(double)config.CollidedBallsAreaRateDen;
	MinSquareDistanceToCreateNewBall = config.MinSquareDistanceToCreateNewBall;
	FramesNeededToDropBall = config.FramesNeededToDropBall;
	return;
}

int BallTracker::SquareDistance(Point2i &First, Point2i &Second){
	return (First.x - Second.x)*(First.x - Second.x) + (First.y - Second.y)*(First.y - Second.y);
}

int BallTracker::FindClosestVisibleBall(Point2i &NewBall,bool UsePredict)
{
	if (UsePredict)
	{
		int ClosestBall = 0;
		if (Balls.empty()) return -1;
		else
		{
			for (unsigned int i = 1; i < Balls.size(); i++)
			{
				if (Balls[i].GetVisible())
				if (SquareDistance(NewBall, Balls[i].PredictPosition())<SquareDistance(NewBall, Balls[ClosestBall].PredictPosition()))
					ClosestBall = i;
			}
			if (SquareDistance(NewBall, Balls[ClosestBall].PredictPosition()) > MinSquareDistanceToCreateNewBall)
				return -1;
		}
		return ClosestBall;
	}
	else
	{
		int ClosestBall = 0;
		if (Balls.empty()) return -1;
		else
		{
			for (unsigned int i = 1; i < Balls.size(); i++)
			{
				if (Balls[i].GetVisible())
				if (SquareDistance(NewBall, Balls[i].GetPosition())<SquareDistance(NewBall, Balls[ClosestBall].GetPosition()))
					ClosestBall = i;
			}
			if (SquareDistance(NewBall, Balls[ClosestBall].GetPosition()) > 2500)
				return -1;
		}
		return ClosestBall;
	}
}

int BallTracker::FindSecondClosestVisileBall(Point2i &NewBall, bool UsePredict)
{
	int ClosestBall = 0, SecondClosestBall = 0;
	if (Balls.empty()) return -1;
	else
	{
		if (SquareDistance(NewBall, Balls[0].GetPosition()) < SquareDistance(NewBall, Balls[1].GetPosition()))
		{
			SecondClosestBall = 1;
		}
		else ClosestBall = 1;
		for (unsigned int i = 2; i < Balls.size(); i++)
		{
			if (Balls[i].GetVisible())
			{
				if (SquareDistance(NewBall, Balls[i].GetPosition()) < SquareDistance(NewBall, Balls[ClosestBall].GetPosition()))
				{
					SecondClosestBall = ClosestBall;
					ClosestBall = i;
				}
				else
				{
					if (SquareDistance(NewBall, Balls[i].GetPosition()) < SquareDistance(NewBall, Balls[SecondClosestBall].GetPosition()))
						SecondClosestBall = i;
				}
			}
		}
	}
	return SecondClosestBall;
}

void BallTracker::FindBallContoursUsingHSV(Mat& img)
{
	Mat imgThresh, imgHSV;
	cvtColor(img, imgHSV, CV_BGR2HSV);
	inRange(imgHSV, HSVlow, HSVhigh, imgThresh);
	findContours(imgThresh, Contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
}

bool BallTracker::CollisionDetection(Point2f &center, float &radius, int &ClosestVisibleBall)
{
	if (radius < CollidedBallsAreaRate*Balls[ClosestVisibleBall].GetRadius() || Balls[ClosestVisibleBall].GetSize() < 5)
		return false;
	else
	{
		int SecondClosestVisibleBall = FindSecondClosestVisileBall((Point2i)center);
		if (ClosestVisibleBall>SecondClosestVisibleBall)
		{
		int exchange = ClosestVisibleBall;
		ClosestVisibleBall = SecondClosestVisibleBall;
		SecondClosestVisibleBall = exchange;
		}
		for (unsigned int i = 0; i < Collisions.size(); i++)
		{
			if (Collisions[i].x == ClosestVisibleBall && Collisions[i].y == SecondClosestVisibleBall) return true;
		}
		Balls[ClosestVisibleBall].SetVisible(false);
		Balls[SecondClosestVisibleBall].SetVisible(false);
		Point2i NewCollision(ClosestVisibleBall, SecondClosestVisibleBall);
		Collisions.push_back(NewCollision);
		CollisionsPlace.push_back(Balls[ClosestVisibleBall].GetPosition());
	//	waitKey(0);
		return true;
	}
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
			int ClosestVisibleBall = FindClosestVisibleBall((Point2i)center, UsePredict);
			if (ClosestVisibleBall == -1) // új labda
			{
				Ball NewBall((int)center.x, (int)center.y, (int)radius);
				Balls.push_back(NewBall);
			}
			else
			{
				if (!CollisionDetection(center, radius, ClosestVisibleBall))
					Balls[ClosestVisibleBall].UpdateData((int)center.x, (int)center.y, (int)radius);
			}
			circle(fortesting, (Point2i)center, (int)radius, Scalar(255, 0, 0)); // for testing
		}
	}
}

void BallTracker::DrawVisibleBallRoutes(Mat &img)
{
	for (unsigned int i = 0; i < Balls.size(); i++)
	{
		if (Balls[i].GetLastSeen()>FramesNeededToDropBall) Balls[i].SetVisible(false);
		if (Balls[i].GetVisible()) Balls[i].DrawLine(img);
	}
}

void BallTracker::processFrame(Mat& img){
	/*HSV:0-180, 0-97, 201-256 */
	FindBallContoursUsingHSV(img);
	MatchContoursWithBalls(img);
	DrawVisibleBallRoutes(img);
	for (unsigned int i = 0; i < Collisions.size(); i++)
	{	
		circle(img, CollisionsPlace[i],15, Scalar(0, 255, 0), -1);
	}
	imshow("pic", img);
	//if (Collisions.size()>0) waitKey(0);
	//waitKey(0);
}



BallTracker::~BallTracker() {
}