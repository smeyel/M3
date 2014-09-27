#pragma once

#include "CameraRemoteProxy.h"
#include "ObjectTracker.h"
#include "ObjectMatcher.h"
#include "BallMatcher.h"
#include "LaserPointerTracker.h"
#include "TrackerCamera.h"
#include <fstream>

class RemoteTrackerDevice
{
public:
	bool running;

	/*
	Cameras used by the device.
	*/
	vector<TrackerCamera*> trackCams;
	ObjectMatcher* pObjectMatcher;
	RemoteTrackerDevice();
	virtual ~RemoteTrackerDevice();

	/*
	Initilaizing the cameras. You have to add manually the cameras you want to use.
	TODO: reads cameras dinamically from init file.
	*/
	virtual bool init(const char* initFile);

	/*
	Connect to all cameras.
	*/
	virtual void connectToAllCamera();

	/*
	Disconnects from all cameras.
	*/
	virtual void disconnectFromAllCamera();

	/*
	Starts the tracking process on the TrackerCameras.
	*/
	virtual void startTracking();
};

