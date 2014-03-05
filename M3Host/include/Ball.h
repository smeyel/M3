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
	Point2i PredictedPosition;
	bool visible;
public:
	Ball();
	void DrawLine(Mat &img);
	void UpdatePosition(Point2i &pos);
	void PredictPosition();
	void SetVisible(bool isVisible);
	bool GetVisible();
	~Ball();
};
#endif