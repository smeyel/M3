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
	FramesNeededToDetectCollision = config.FramesNeededToDetectCollision;
	FramesNeededToDropBall = config.FramesNeededToDropBall;
	ContourMinSize = config.ContourMinSize;

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
			while (!Balls[ClosestBall].GetVisible())
			{
				if (ClosestBall == (Balls.size() - 1)) return -1;
				ClosestBall++;
			}
		
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
			while (!Balls[ClosestBall].GetVisible())
			{
				if (ClosestBall == (Balls.size() - 1)) return -1;
				ClosestBall++;
			}
			for (unsigned int i = 1; i < Balls.size(); i++)
			{
				if (Balls[i].GetVisible())
				if (SquareDistance(NewBall, Balls[i].GetPosition())<SquareDistance(NewBall, Balls[ClosestBall].GetPosition()))
					ClosestBall = i;
			}
			if (SquareDistance(NewBall, Balls[ClosestBall].GetPosition()) > MinSquareDistanceToCreateNewBall)
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

bool BallTracker::CollisionDetection(Point2f &center, float &radius, int &ClosestVisibleBall, int ContourIndex, Mat& fortesting)
{
	if (radius < CollidedBallsAreaRate*Balls[ClosestVisibleBall].GetRadius() || Balls[ClosestVisibleBall].GetSize() < FramesNeededToDropBall)
		return false;
	else
	{
		int SecondClosestVisibleBall = FindSecondClosestVisileBall((Point2i)center);
	/*	if (ClosestVisibleBall>SecondClosestVisibleBall)
		{
		int exchange = ClosestVisibleBall;
		ClosestVisibleBall = SecondClosestVisibleBall;
		SecondClosestVisibleBall = exchange;
		}
		for (unsigned int i = 0; i < Collisions.size(); i++)
		{
			if (Collisions[i].x == ClosestVisibleBall && Collisions[i].y == SecondClosestVisibleBall) return true;
		}*/
		vector<Point2i> ClosestBallContour;
		vector<Point2i> SecondClosestBallContour;
		for (unsigned int i = 0; i < Contours[ContourIndex].size(); i++)
		{
			if (SquareDistance(Contours[ContourIndex][i], Balls[ClosestVisibleBall].PredictPosition())<
				SquareDistance(Contours[ContourIndex][i], Balls[SecondClosestVisibleBall].PredictPosition()))
				ClosestBallContour.push_back(Contours[ContourIndex][i]);
			else SecondClosestBallContour.push_back(Contours[ContourIndex][i]);
		}
		float radius;
		Point2f center;
		minEnclosingCircle(ClosestBallContour, center, radius);
		Balls[ClosestVisibleBall].UpdateData((int)center.x, (int)center.y, Balls[ClosestVisibleBall].GetRadius());
		circle(fortesting, center, radius, Scalar(0, 0, 255),3);
		minEnclosingCircle(SecondClosestBallContour, center, radius);
		Balls[SecondClosestVisibleBall].UpdateData((int)center.x, (int)center.y, Balls[SecondClosestVisibleBall].GetRadius());
		circle(fortesting, center, radius, Scalar(0, 255, 0),3);
		/*Point2i NewCollision(ClosestVisibleBall, SecondClosestVisibleBall);
		Collisions.push_back(NewCollision);
		CollisionsPlace.push_back(Balls[ClosestVisibleBall].GetPosition());*/
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
		if (Contours[i].size()>ContourMinSize)
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
				if (!CollisionDetection(center, radius, ClosestVisibleBall,i,fortesting))
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