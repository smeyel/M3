#include "TrackerCamera.h"
#include "TrackerConfigManager.h"
#include "CameraRemoteConfigManager.h"


TrackerCamera::TrackerCamera()
{
	camProxy = NULL;
	tracker = NULL;
	running = false;
	saveToFile = false;
}

bool TrackerCamera::init(const char* cameraInitFileName, const char* trackerInitFileName){
	camProxy = new CameraRemoteProxy();
	this->needinit = true;
	bool initialized = true;
		if (!camProxy->camera->loadCalibrationData("ps3eye_intrinsics_blu.xml")){
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

void TrackerCamera::savingToFile(int index)
{
	string tempDestination;
	VideoWriter oVideoWriter;
	/* Save To File
	For help look http://opencv-srf.blogspot.hu/2011/09/saving-images-videos_16.html */
	double dWidth = 640; //get the width of frames of the video
	double dHeight = 480; //get the height of frames of the video
	cout << "Frame Size = " << dWidth << "x" << dHeight << endl;
	Size frameSize(static_cast<int>(dWidth), static_cast<int>(dHeight));
	tempDestination = "cam_";
	tempDestination += to_string(index);
	tempDestination += "_";
	tempDestination += destination;
	oVideoWriter = VideoWriter(tempDestination, CV_FOURCC('M', 'P', '4', '2'), 20, frameSize, true); //initialize the VideoWriter object 00
	if (!oVideoWriter.isOpened()) //if not initialize the VideoWriter successfully, exit the program
	{
		cout << "ERROR: Failed to write the video" << endl;
		return;
	}
	for (unsigned int i = 0; i < VideoPuffer.size(); i++)
	{
		oVideoWriter.write(VideoPuffer[i]);
	}
	oVideoWriter.release();
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
		return true;
	}
	else
	{
		imshow(TrackerCameraName, *camProxy->lastImageTaken);
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
