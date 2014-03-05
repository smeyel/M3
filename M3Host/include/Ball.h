#ifndef __BALL_H
#define __BALL_H

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/core.hpp>

#include <vector>

using namespace cv;
using namespace std;

class Ball
{
	vector<Point2i> Position;
	bool visible;
public:
	Ball();
	Ball(Point2i &StartPoint);
	void DrawLine(Mat &img);
	int GetSize();
	Point2i GetPosition();
	void UpdatePosition(Point2i &pos);
	Point2i PredictPosition();
	void SetVisible(bool isVisible);
	bool GetVisible();
	~Ball();
};
#endif