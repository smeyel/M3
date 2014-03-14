#ifndef __BALLTRACKERCONFIGMANAGER_H_
#define __BALLTRACKERCONFIGMANAGER_H_
#define _WINSOCKAPI_
#include <iostream>
#include <stdlib.h>
#include "SimpleIniConfigReader.h"

using namespace LogConfigTime;
using namespace std;

class BallTrackerConfigManager
{
	virtual bool readconfigdata(const char* filename)
	{
		SimpleIniConfigReader *SIreader = new SimpleIniConfigReader(filename);
		ConfigReader *reader = SIreader;


		Hlow = reader->getIntValue("ball", "Hlow");
		Slow = reader->getIntValue("ball", "Slow ");
		Vlow = reader->getIntValue("ball", "Vlow");
		Hhigh = reader->getIntValue("ball", "Hhigh");
		Shigh = reader->getIntValue("ball", "Shigh");
		Vhigh = reader->getIntValue("ball", "Vhigh");
		CollidedBallsAreaRateNum = reader->getIntValue("ball", "CollidedBallsAreaRateNum");
		CollidedBallsAreaRateDen = reader->getIntValue("ball", "CollidedBallsAreaRateDen");
		MinSquareDistanceToCreateNewBall = reader->getIntValue("ball", "MinSquareDistanceToCreateNewBall");
		FramesNeededToDetectCollision = reader->getIntValue("ball", "FramesNeededToDetectCollision");
		FramesNeededToDropBall = reader->getIntValue("ball", "FramesNeededToDropBall");
		ContourMinSize = reader->getIntValue("ball", "ContourMinSize");
		SquareDistanceToInvolveCollision = reader->getIntValue("ball", "SquareDistanceToInvolveCollision");

		delete SIreader;
		return true;

	}

public:

	BallTrackerConfigManager(const char* filename)
	{
		readconfigdata(filename);
	}

	int Hlow;
	int Slow;
	int Vlow;
	int Hhigh;
	int Shigh;
	int Vhigh;
	int CollidedBallsAreaRateNum;
	int CollidedBallsAreaRateDen;
	int MinSquareDistanceToCreateNewBall;
	int FramesNeededToDetectCollision;
	int FramesNeededToDropBall;
	int ContourMinSize;
	int SquareDistanceToInvolveCollision;

};


#endif