#include "Ball.h"

using namespace cv;

Ball::Ball(){
	LastSeen = 0;
	SetUpdated(false);
	UseKalmanFilter = 0;
}

Ball::Ball(int x, int y, int r, int UseKalmanFilter){
	Point3i Point(x, y, r);
	BallData.push_back(Point);

	visible = true;
	LastSeen = 0;
	SetUpdated(true);
	this->UseKalmanFilter = UseKalmanFilter;
	if (this->UseKalmanFilter)
	{
		KF.init(6, 2, 0);
		Mat_<float> mes(2, 1); mes.setTo(Scalar(0));
		this->measurement = mes;
		KF.statePre.at<float>(0) = x;
		KF.statePre.at<float>(1) = y;
		KF.statePre.at<float>(2) = 0;
		KF.statePre.at<float>(3) = 0;
		KF.statePre.at<float>(4) = 0;
		KF.statePre.at<float>(5) = 0;
		KF.transitionMatrix = *(Mat_<float>(6, 6) << 1, 0, 1, 0, 0.5, 0, 0, 1, 0, 1, 0, 0.5, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1);

		setIdentity(KF.measurementMatrix);
		setIdentity(KF.processNoiseCov, Scalar::all(1e-4));
		setIdentity(KF.measurementNoiseCov, Scalar::all(1e-3));
		setIdentity(KF.errorCovPost, Scalar::all(.1));
		BallData_kalman.push_back(Point);
	}
}

int Ball::GetSize()
{
	return BallData.size();
}
void Ball::DrawLine(Mat &img)
{
	for (unsigned int i = 0; i < BallData.size() - 1; i++)
	{
		Point2i Pointi(BallData[i].x, BallData[i].y);
		Point2i Pointj(BallData[i + 1].x, BallData[i + 1].y);
		line(img, Pointi,Pointj, Scalar(255, 0, 0),2);
		if (UseKalmanFilter)
		{
			Point2i Pointi(BallData_kalman[i].x, BallData_kalman[i].y);
			Point2i Pointj(BallData_kalman[i + 1].x, BallData_kalman[i + 1].y);
			line(img, Pointi, Pointj, Scalar(0, 0, 255), 2);
		}
	}
}
void Ball::DrawLine_kalman(Mat& img)
{
	for (unsigned int i = 0; i < BallData_kalman.size() - 1; i++)
	{
		Point2i Pointi(BallData_kalman[i].x, BallData_kalman[i].y);
		Point2i Pointj(BallData_kalman[i + 1].x, BallData_kalman[i + 1].y);
		line(img, Pointi, Pointj, Scalar(0, 0, 255), 2);
	}
}

Point2i Ball::GetPosition()
{
	if (BallData.empty()) return (Point2i)(-1, -1);
	Point2i Point(BallData.back().x, BallData.back().y);
	return Point;
}

int Ball::GetRadius()
{
	return BallData.back().z;
}

void Ball::UpdateData(int x, int y, int r)
{
	Point3i Point(x, y, r);
	BallData.push_back(Point);
	SetUpdated(true);
	DecrementLastSeen();
	if (UseKalmanFilter)
	{
		KF.predict();
		measurement(0) = x;
		measurement(1) = y;
		Mat estimated = KF.correct(measurement);
		Point3i newdata_kalman(estimated.at<float>(0), estimated.at<float>(1), r);
		BallData_kalman.push_back(newdata_kalman);
	}
}

Point2i Ball::PredictPosition()
{
	if (BallData.size() > 1)
	{
		Point2i Predicted(2 * BallData[BallData.size() - 1].x - BallData[BallData.size() - 2].x,
			2 * BallData[BallData.size() - 1].y - BallData[BallData.size() - 2].y);
		return Predicted;
	}
	else return this->GetPosition();
}

Point2i Ball::PredictPositionWithKalmanFilter()
{
	Mat prediction = KF.predict();
	Point2i predictPt(prediction.at<float>(0), prediction.at<float>(1));
	return predictPt;
}
void Ball::SetVisible(bool isVisible)
{
	visible = isVisible;
}
bool Ball::GetVisible()
{
	return visible;
}

void Ball::IncrementLastSeen()
{
	LastSeen++;
}

void Ball::DecrementLastSeen()
{
	LastSeen--;
}

int Ball::GetLastSeen()
{
	return LastSeen;
}

void Ball::DeInit()
{
	this->IncrementLastSeen();
	this->SetUpdated(false);
}

void Ball::SetUpdated(bool updated)
{
	this->updated = updated;
}

bool Ball::GetUpdated()
{
	return updated;
}

Ball::~Ball(){

}