#include "Ball.h"

using namespace cv;

Ball::Ball(){
	
}

Ball::Ball(Point2i &StartPoint){
	Position.push_back(StartPoint);
	visible = true;
}

int Ball::GetSize()
{
	return Position.size();
}
void Ball::DrawLine(Mat &img)
{
	for (int i = 0; i < Position.size()-1; i++)
	{
		line(img, Position[i], Position[i + 1], Scalar(255, 0, 0));
	}
}

Point2i Ball::GetPosition()
{
	if (Position.empty()) return (Point2i)(-1, -1);
	return Position.back();
}

void Ball::UpdatePosition(Point2i &pos)
{
	Position.push_back(pos);
}

Point2i Ball::PredictPosition()
{
	if (Position.size()>1) return 2 * Position[Position.size() - 1] - Position[Position.size() - 2];
}

void Ball::SetVisible(bool isVisible)
{
	visible = isVisible;
}
bool Ball::GetVisible()
{
	return visible;
}

Ball::~Ball(){

}