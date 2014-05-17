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
	SquareDistanceToInvolveCollision = config.SquareDistanceToInvolveCollision;
	ErosionSize = config.ErosionSize;
	UseErosion = config.UseErosion;
	UsePrediction = config.UsePrediction;
	UseKalmanFilter = config.UseKalmanFilter;
	BallsSearchContours = config.BallsSearchContours;

	return;
}

int BallTracker::SquareDistance(Point2i &First, Point2i &Second){
	return (First.x - Second.x)*(First.x - Second.x) + (First.y - Second.y)*(First.y - Second.y);
}
int BallTracker::FindClosestContour(int BallID)
{
	int ClosestContour = 0;
	if (Contours.empty()) return -1;
	else
	{
		for (unsigned int i = 1; i < Contours.size(); i++)
		{
			if (this->UsePrediction)
			{
				if (SquareDistance(Balls[BallID].PredictPosition(), ContourCenter[i]) < SquareDistance(Balls[BallID].PredictPosition(), ContourCenter[ClosestContour]))
					ClosestContour = i;
			}
			else
			{
				if (SquareDistance(Balls[BallID].GetPosition(), ContourCenter[i]) < SquareDistance(Balls[BallID].GetPosition(), ContourCenter[ClosestContour]))
					ClosestContour = i;
			}
		}
		if (SquareDistance(Balls[BallID].GetPosition(), ContourCenter[ClosestContour]) > MinSquareDistanceToCreateNewBall)
			return -1;
	}
	return ClosestContour;
}
int BallTracker::FindClosestVisibleBall(Point2i &NewBall)
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
			{
				if (UsePrediction)
				{
					if (SquareDistance(NewBall, Balls[i].PredictPosition())<SquareDistance(NewBall, Balls[ClosestBall].PredictPosition()))
						ClosestBall = i;
				}
				else
				{
					if (SquareDistance(NewBall, Balls[i].GetPosition())<SquareDistance(NewBall, Balls[ClosestBall].GetPosition()))
						ClosestBall = i;
				}
			}
		}
		if (SquareDistance(NewBall, Balls[ClosestBall].PredictPosition()) > MinSquareDistanceToCreateNewBall)
			return -1;
	}
	return ClosestBall;
}

int BallTracker::FindSecondClosestVisileBall(Point2i &NewBall)
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
	if (UseErosion) ErodeFrame(imgThresh);
	imshow("Thresh", imgThresh);
	findContours(imgThresh, Contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
}
void BallTracker::CalculateContourParams()
{
	vector<Point2i> ContourCenterTemp;
	vector<int> ContourRadiusTemp;
	for (int i = 0; i < Contours.size(); i++)
	{
		if (Contours[i].size()<ContourMinSize)
		{
			Contours.erase(Contours.begin() + i);
			i--; 
		}
		else
		{
			Point2f center;
			float radius;
			minEnclosingCircle(Contours[i], center, radius);
			ContourCenterTemp.push_back((Point2i)center);
			ContourRadiusTemp.push_back((int)radius);
		}
	}
	this->ContourCenter = ContourCenterTemp;
	this->ContourRadius = ContourRadiusTemp;
}

bool BallTracker::CollisionDetection(Point2i &center, int &radius, int &ClosestVisibleBall, int ContourIndex, Mat& fortesting)
{
	if (radius < CollidedBallsAreaRate*Balls[ClosestVisibleBall].GetRadius() || Balls[ClosestVisibleBall].GetSize() < FramesNeededToDetectCollision)
		return false;
	else
	{
		vector<int> BallsInCollision;
		vector<vector<Point2i>> SeparatedContours;
		for (int i = 0; i < Balls.size(); i++)
		{
			if ((Balls[i].GetVisible()&&!Balls[i].GetUpdated()) && SquareDistance((Point2i)center, Balls[i].GetPosition()) < SquareDistanceToInvolveCollision)
			{
				BallsInCollision.push_back(i);
				vector<Point2i> SingleContour;
				SeparatedContours.push_back(SingleContour);
			}
		}
		

	//	int ClosestBallInCollision;
		for (unsigned int i = 0; i < Contours[ContourIndex].size(); i++)
		{
			int ClosestBallInCollision=0;
			for (unsigned j = 0; j < BallsInCollision.size(); j++)
			{
				if (SquareDistance(Balls[BallsInCollision[ClosestBallInCollision]].PredictPosition(), Contours[ContourIndex][i])>
					SquareDistance(Balls[BallsInCollision[j]].PredictPosition(), Contours[ContourIndex][i]))
					ClosestBallInCollision = j;
			}
			SeparatedContours[ClosestBallInCollision].push_back(Contours[ContourIndex][i]);
		}
		//TODO: Mi van ha nincs legközelebbi labada!

		for (unsigned int i = 0; i < BallsInCollision.size(); i++)
		{
			if (SeparatedContours[i].size()>10)
			{
				float radius;
				Point2f center;
				minEnclosingCircle(SeparatedContours[i], center, radius);
				cv::circle(fortesting, center, radius, Scalar(0, 255, 0), 2);
				Balls[BallsInCollision[i]].UpdateData((int)center.x, (int)center.y, Balls[BallsInCollision[i]].GetRadius());
			}

		}
		return true;
	}
}

void BallTracker::MatchBallsWithContours(Mat &fortesting)
{
	for (unsigned int i = 0; i < Balls.size(); i++)
	{
		Balls[i].DeInit();
	}
	for ( int i = 0; i < Balls.size(); i++)
	{
		if (Balls[i].GetVisible() && !Balls[i].GetUpdated())
		{
			int ClosestContour = FindClosestContour(i);
			if (ClosestContour == -1) // nincs közeli kontúr
			{
				Balls[i].IncrementLastSeen();
			}
			else
			{
				int ClosestVisibleBall = FindClosestVisibleBall(ContourCenter[ClosestContour]);
				//TODO ClosestVisibleBall=-1 miért lesz (tippre, ha a labda kigurul a képrõl, és egy másik kapja meg az útvonalát
				if (ClosestVisibleBall!=-1 && !CollisionDetection(ContourCenter[ClosestContour], ContourRadius[ClosestContour], ClosestVisibleBall, ClosestContour, fortesting))
				{
					Balls[i].UpdateData(ContourCenter[ClosestContour].x, ContourCenter[ClosestContour].y, ContourRadius[ClosestContour]);
					circle(fortesting, ContourCenter[ClosestContour], ContourRadius[ClosestContour], Scalar(255, 0, 0), 2); // for testing
				}
				Contours.erase(Contours.begin() + ClosestContour);
				ContourCenter.erase(ContourCenter.begin() + ClosestContour);
				ContourRadius.erase(ContourRadius.begin() + ClosestContour);
			}
		}
		
	}
	for (unsigned int i = 0; i < Contours.size(); i++)
	{
		Ball NewBall(ContourCenter[i].x, ContourCenter[i].y, ContourRadius[i],UseKalmanFilter);
		Balls.push_back(NewBall);
		circle(fortesting, ContourCenter[i], ContourRadius[i], Scalar(255, 0, 0), 2); // for testing
	}
}
void BallTracker::MatchContoursWithBalls(Mat& fortesting)
{
	for (unsigned int i = 0; i < Balls.size(); i++)
	{
		Balls[i].DeInit();
	}

	for (unsigned int i = 0; i < Contours.size(); i++)
	{
		int ClosestVisibleBall = FindClosestVisibleBall(ContourCenter[i]);
		if (ClosestVisibleBall == -1) // új labda
		{
			Ball NewBall(ContourCenter[i].x, ContourCenter[i].y, ContourRadius[i],UseKalmanFilter);
			Balls.push_back(NewBall);
		}
		else
		{
			if (!CollisionDetection(ContourCenter[i], ContourRadius[i], ClosestVisibleBall, i, fortesting))
				Balls[ClosestVisibleBall].UpdateData(ContourCenter[i].x, ContourCenter[i].y, ContourRadius[i]);
		}
		circle(fortesting, ContourCenter[i],ContourRadius[i], Scalar(255, 0, 0), 2); // for testing
	}
}

void BallTracker::DrawVisibleBallRoutes(Mat &img)
{
	for (unsigned int i = 0; i < Balls.size(); i++)
	{
		if (Balls[i].GetLastSeen()>FramesNeededToDropBall) Balls[i].SetVisible(false);
		if (Balls[i].GetVisible())
		{
			Balls[i].DrawLine(img);
		}
	}
}

void BallTracker::ErodeFrame(Mat& img)
{
	Mat element = cv::getStructuringElement(cv::MORPH_RECT,
		cv::Size(2 * ErosionSize + 1, 2 * ErosionSize + 1),
		cv::Point(ErosionSize, ErosionSize));
	imshow("before", img);
	erode(img, img, element);
	imshow("after", img);
}

void BallTracker::processFrame(Mat& img){
	FindBallContoursUsingHSV(img);
	CalculateContourParams();
	if (BallsSearchContours) MatchBallsWithContours(img);
	else MatchContoursWithBalls(img);
	DrawVisibleBallRoutes(img);
	imshow("pic", img);
	waitKey(30);
}

Point2f BallTracker::getLastPoint(){ return Point2f(); }

BallTracker::~BallTracker() {
}