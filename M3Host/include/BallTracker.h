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

	vector<vector<Point>> kontur;
	vector<Ball> Balls;
	Mat imgHSV, imgThresh;

public:
	BallTracker();

	void processFrame(Mat& img);
	void init(const char *configfilename);

	~BallTracker();
};

#endif