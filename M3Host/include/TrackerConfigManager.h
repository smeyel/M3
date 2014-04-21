#ifndef __TRACKERCONFIGMANAGER_H_
#define __TRACKERCONFIGMANAGER_H_
#include <iostream>
#include <string>
#include <stdlib.h>
#include "SimpleIniConfigReader.h"


using namespace LogConfigTime;
using namespace std;

enum BallType {
	LASER, PINGPONG
};
enum BallColor {
	RED, BLUE
};
class TrackerConfigManager
{
public:
	// Tracker settings:
	BallType trackerType;
	BallColor trackerColor;
	 
	TrackerConfigManager()
	{
	}

	TrackerConfigManager(const char* trackerInitFilename)
	{
		readTrackerConfiguration(trackerInitFilename);
	}
	


virtual bool readTrackerConfiguration(const char* filename)
{
	SimpleIniConfigReader *SIreader = new SimpleIniConfigReader(filename);
	ConfigReader *reader = SIreader;

	switch (reader->getIntValue("main", "trackerType"))
	{
	case 0:
		trackerType = LASER;
		break;
	case 1:
		trackerType = PINGPONG;
		break;
	default:
		cout << "Invalid tracker type in " << filename << endl;
		exit(-1);
	}

	switch (reader->getIntValue("main", "trackerColor"))
	{
	case 0:
		trackerColor = RED;
		break;
	case 1:
		trackerColor = BLUE;
		break;
	default:
		cout << "Invalid tracker type in " << filename << endl;
		exit(-1);
	}



	delete SIreader;
	return true;



}

};

#endif