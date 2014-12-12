#include "TrackerCamera.h"
#include "TrackerConfigManager.h"
#include "CameraRemoteConfigManager.h"


TrackerCamera::TrackerCamera()
{
	camProxy = NULL;
	tracker = NULL;
	running = false;
	saveToFile = false;
	isCalibrated = false;
}

bool TrackerCamera::init(const char* cameraInitFileName, const char* trackerInitFileName){
	camProxy = new CameraRemoteProxy();
	this->needinit = true;
	bool initialized = true;
		if (!camProxy->camera->loadCalibrationData("intrinsic_n8_3.xml")){
			cout << "Camera calibration failed!!" << endl;
			initialized = false;
		}
	
	//loading tracker configFile from initFile
	config_tracker.readTrackerConfiguration(trackerInitFileName);



	//create the given tracker class:
	switch (config_tracker.trackerType)
	{
	case LASER:
		tracker = new LaserPointerTracker();
		tracker->init(trackerInitFileName);
		break;

	case PINGPONG:
		tracker = new BallTracker();
		tracker->init(trackerInitFileName);
		break;
	}


	//loading camera remote settings:
	CameraRemoteConfigManager config_cam(cameraInitFileName);
	port = config_cam.port;
	ip_addr = config_cam.ip_addr;
	char temp[30];
	sprintf_s(temp, 25, "%s:%d", ip_addr.c_str(), port);
	TrackerCameraName = string(temp);
	saveToFile = config_cam.saveToFile;
	destination = config_cam.destination;
	//TODO: calling tracker.init() it will call the trracker implementation's method
	TimeReference = 0;
	return initialized;
}

void TrackerCamera::connect(){
	camProxy->Connect(ip_addr.c_str(), port);
}

bool TrackerCamera::calibrate()
{
	waitKey(30);
	if (camProxy->CaptureAndTryCalibration(true))
	{
		imshow(TrackerCameraName, *camProxy->lastImageTaken);
		isCalibrated = true;
		return true;
	}
	else
	{
		imshow(TrackerCameraName, *camProxy->lastImageTaken);
		isCalibrated = false;
		return false;
	}
}
void TrackerCamera::disconnect(){
	camProxy->Disconnect();
}

void TrackerCamera::processFrame(){
	camProxy->CaptureImage();
	tracker->processFrame(*camProxy->lastImageTaken);
	tracker->drawOnImage(*camProxy->lastImageTaken, TrackerCameraName);
	
}


TrackerCamera::~TrackerCamera()
{

	if (camProxy)
	{
		delete camProxy;
		camProxy = NULL;
	}
	if (tracker)
	{
		delete tracker;
		tracker = NULL;
	}
}
