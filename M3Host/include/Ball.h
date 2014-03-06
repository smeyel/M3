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
	vector<Point3i> BallData;
	bool visible;
public:
	Ball();
	Ball(int x, int y, int z);
	void DrawLine(Mat &img);
	int GetSize();
	Point2i GetPosition();
	void UpdateData(int x, int y, int r);
	Point2i PredictPosition();
	void SetVisible(bool isVisible);
	bool GetVisible();
	~Ball();
};
#endif