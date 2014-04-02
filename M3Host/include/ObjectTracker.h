#ifndef __OJECTTRACKER_H_
#define __OJECTTRACKER_H_


#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>

#include "TimeMeasurement.h"
#include "camera.h"
using namespace cv;
using namespace LogConfigTime;

/*
Tracker Base interface.
*/
class ObjectTracker
{
public:
	Camera *camera;
	TimeMeasurement *timeMeasurement=NULL;
	ObjectTracker();

	/*
	Initializing the tracker from the given file.
	*/
	virtual void init(const char *configfilename)=0;

	/*
	Returns the last found point.
	*/
	virtual Point2f getLastPoint()=0;

	/*
	Processing a frame with the tracker.
	*/
	virtual void processFrame(Mat &src)=0;

	~ObjectTracker();
};

#endif