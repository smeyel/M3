#ifndef __BALLTRACKER_H
#define __BALLTRACKER_H

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/opencv.hpp>


#include "ObjectTracker.h"
#include "DetectionParameters.h"
#include "Ball.h"

using namespace cv;
using namespace std;

class BallTracker : public ObjectTracker{

	vector<vector<Point2i>> Contours;
	vector<Ball> Balls;

public:
	BallTracker();

	void processFrame(Mat& img);
	void init(const char *configfilename);
	void FindBallContoursUsingHSV(Mat& img, Scalar Low, Scalar High);
	void MatchContoursWithBalls(Mat &fortesting, bool UsePredict = false);
	int SquareDistance(Point2i &First, Point2i &Second);
	int FindClosestVisibleBall(Point2i &NewBall, bool UsePredict = false);
	void DrawVisibleBallRoutes(Mat &img);
	~BallTracker();
};

#endif