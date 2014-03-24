#include "Ball.h"

using namespace cv;

Ball::Ball(){
	LastSeen = 0;
}

Ball::Ball(int x, int y, int r){
	Point3i Point(x, y, r);
	BallData.push_back(Point);
	visible = true;
	LastSeen = 0;
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
	DecrementLastSeen();
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

Ball::~Ball(){

}