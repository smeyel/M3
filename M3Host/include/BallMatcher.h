#ifndef __BALLMATCHER_H
#define __BALLMATCHER_H

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>
#include "ObjectMatcher.h"
#include "BallTracker.h"
#include "TimeMeasurement.h"
#include "camera.h"
using namespace cv;

class BallMatcher : public ObjectMatcher
{
	vector<vector<Ball>*> BallsToMatchData;
	vector<vector<int>> LastPairIndexes;
	vector<vector<int>> PairIndexes;
public:
	BallMatcher();
	BallMatcher(vector<Camera*> cameras, vector<ObjectsToMatch*> objectsToMatch, vector<Mat*> images, vector<string*> names);
	void init(const char *configfilename);
	void MatchBalls();
	void DrawOnImages();
	void MatchObjects();
	~BallMatcher();
};

#endif