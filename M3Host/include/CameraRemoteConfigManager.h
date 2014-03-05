#ifndef __CAMERAREMOTECONFIGMANAGER_H_
#define __CAMERAREMOTECONFIGMANAGER_H_
#include <iostream>
#include <stdlib.h>
#include "SimpleIniConfigReader.h"
#include "DetectionParameters.h"

using namespace LogConfigTime;
using namespace std;


class CameraRemoteConfigManager
{
	virtual bool readCameraConfiguration(const char *filename)
	{
		SimpleIniConfigReader *SIreader = new SimpleIniConfigReader(filename);
		ConfigReader *reader = SIreader;

		ip_addr = reader->getStringValue("main", "proxyIP");
		port = reader->getIntValue("main", "proxyPort");

		delete SIreader;
		return true;
	}

public:
	
	CameraRemoteConfigManager(const char* cameraInitFilename)
	{
		readCameraConfiguration(cameraInitFilename);
	}

	//Remote proxy settings:
	std::string ip_addr;
	int port;


};



#endif