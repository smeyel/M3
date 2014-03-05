#ifndef __OJECTTRACKER_H_
#define __OJECTTRACKER_H_


#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>

#include "TimeMeasurement.h"
using namespace cv;
using namespace LogConfigTime;
class ObjectTracker
{
public:
	TimeMeasurement *timeMeasurement=NULL;
	ObjectTracker();

	virtual void init(const char *configfilename)=0;
	
	virtual void processFrame(Mat &src)=0;

	~ObjectTracker();
};

#endif