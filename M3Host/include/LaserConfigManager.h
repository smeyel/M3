#ifndef __LASERCONFIGMANAGER_H_
#define __LASERCONFIGMANAGER_H_

#include <iostream>
#include <stdlib.h>
#include "SimpleIniConfigReader.h"
#include "TrackerConfigManager.h"
using namespace LogConfigTime;
using namespace std;

class LaserConfigManager
{
public:
	virtual bool readconfig(const char* filename)
	{
		SimpleIniConfigReader *SIreader = new SimpleIniConfigReader(filename);
		ConfigReader *reader = SIreader;

		laserColor = reader->getIntValue("laser", "laserColor");
		lowerTreshold = reader->getIntValue("laser", "lowerTreshold");
		minArea = reader->getIntValue("laser", "minArea");
		maxArea = reader->getIntValue("laser", "maxArea");
		
		houghParam1 = reader->getIntValue("laser", "houghParam1");
		houghParam2 = reader->getIntValue("laser", "houghParam1");

		minRadius = reader->getIntValue("laser", "minRadius");
		maxRadius = reader->getIntValue("laser", "maxRadius");

		minColorThreshold_R = reader->getIntValue("laser", "minColorThreshold_R");
		minColorThreshold_G = reader->getIntValue("laser", "minColorThreshold_G");
		minColorThreshold_B = reader->getIntValue("laser", "minColorThreshold_B");

		maxColorThreshold_R = reader->getIntValue("laser", "maxColorThreshold_R");
		maxColorThreshold_G = reader->getIntValue("laser", "maxColorThreshold_G");
		maxColorThreshold_B = reader->getIntValue("laser", "maxColorThreshold_B");

		delete SIreader;
		return true;

	}



	LaserConfigManager(const char* filename)
	{
		readconfig(filename);
	}

	LaserConfigManager(){};
	

	int laserColor;
	int lowerTreshold;

	int minArea;
	int maxArea;

	int houghParam1;
	int houghParam2;

	int minRadius;
	int maxRadius;

	int minColorThreshold_R;
	int minColorThreshold_G;
	int minColorThreshold_B;

	int maxColorThreshold_R;
	int maxColorThreshold_G;
	int maxColorThreshold_B;

};


#endif