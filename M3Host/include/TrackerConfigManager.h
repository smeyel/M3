#ifndef __TRACKERCONFIGMANAGER_H_
#define __TRACKERCONFIGMANAGER_H_
#include <iostream>
#include <string>
#include <stdlib.h>
#include "SimpleIniConfigReader.h"
#include "DetectionParameters.h"

using namespace LogConfigTime;
using namespace std;


class TrackerConfigManager
{
	
	virtual bool readTrackerConfiguration(const char* filename)
	{
		SimpleIniConfigReader *SIreader = new SimpleIniConfigReader(filename);
		ConfigReader *reader = SIreader;
		
		switch ( reader->getIntValue("main", "trackerType") )
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
		saveToFile = reader->getBoolValue("main", "saveToFile");
		destination = reader->getStringValue("main", "destination");



		delete SIreader;
		return true;



	}
	 
public:
	
	TrackerConfigManager(const char* trackerInitFilename)
	{
		readTrackerConfiguration(trackerInitFilename);
	}
	// Tracker settings:
	BallType trackerType;
	BallColor trackerColor;
	bool saveToFile;
	string destination;
};



#endif