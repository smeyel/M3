#ifndef __OBJECTMATCHER_H
#define __OBJECTMATCHER_H

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>
#include "ObjectTracker.h"
#include "TimeMeasurement.h"
#include "camera.h"
using namespace cv;

/*
Matcher Base interface.
*/
class ObjectMatcher
{

public:
	vector<Mat*> images;
	vector<string*> names;
	ObjectMatcher();
	ObjectMatcher(vector<ObjectsToMatch*> ObjectsToMatch,vector<Mat*> images, vector<string*> names);
	virtual void init(const char *configfilename) = 0;
	virtual void MatchObjects() = 0;
	~ObjectMatcher();
};

#endif