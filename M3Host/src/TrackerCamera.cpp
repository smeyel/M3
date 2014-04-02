#include "TrackerCamera.h"
#include "TrackerConfigManager.h"
#include "CameraRemoteConfigManager.h"


TrackerCamera::TrackerCamera()
{
	camProxy = NULL;
	tracker = NULL;
	running = false;
	saveToFile = false;//TODO this should be in an initfile!
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

	//save video to file
	saveToFile = config_tracker.saveToFile;
	destination = config_tracker.destination;
	
	if (saveToFile){
		/* Save To File
		For help look http://opencv-srf.blogspot.hu/2011/09/saving-images-videos_16.html */
		double dWidth = 640; //get the width of frames of the video
		double dHeight = 480; //get the height of frames of the video
		cout << "Frame Size = " << dWidth << "x" << dHeight << endl;
		Size frameSize(static_cast<int>(dWidth), static_cast<int>(dHeight));
		oVideoWriter= VideoWriter(destination, CV_FOURCC('M', 'P', '4', '2'), 20, frameSize, true); //initialize the VideoWriter object 00
		if (!oVideoWriter.isOpened()) //if not initialize the VideoWriter successfully, exit the program
		{
			cout << "ERROR: Failed to write the video" << endl;
			return false;
		}
	}
	
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
	//TODO: calling tracker.init() it will call the trracker implementation's method

	return initialized;
}

void TrackerCamera::connect(){
	camProxy->Connect(ip_addr.c_str(), port);

}
void TrackerCamera::disconnect(){
	camProxy->Disconnect();
}

void TrackerCamera::processFrame(){
	if (needinit){
		cv::Mat image(480, 640, CV_8UC3);

		if (config_tracker.trackerType == LASER){
			
			if (!camProxy->CaptureUntilCalibrated(30))
			{
				cout << "Camera is not calibrated!" << endl;
			}
		}
		this->needinit = false;
	}
		
	camProxy->CaptureImage();
	tracker->processFrame(*camProxy->lastImageTaken);
	
	if (saveToFile){
		//Writes the frame into a file.
		oVideoWriter.write(*camProxy->lastImageTaken);
	}

	
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
