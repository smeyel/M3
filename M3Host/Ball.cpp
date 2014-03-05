#include "Ball.h"

using namespace cv;

Ball::Ball(){
	
}

void Ball::DrawLine(Mat &img)
{
	for (int i = 0; i < Position.size()-1; i++)
	{
		line(img, Position[i], Position[i + 1], Scalar(255, 0, 0));
	}
}

void Ball::UpdatePosition(Point2i &pos)
{
	Position.push_back(pos);
	Ball::PredictPosition();
}

void Ball::PredictPosition()
{
	if (Position.size()>1) PredictedPosition = 2 * Position[Position.size() - 1] - Position[Position.size() - 2];
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