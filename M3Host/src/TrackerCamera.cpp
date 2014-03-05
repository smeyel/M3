#include "TrackerCamera.h"
#include "TrackerConfigManager.h"
#include "CameraRemoteConfigManager.h"

TrackerCamera::TrackerCamera()
{
	camProxy = NULL;
	tracker = NULL;
	running = false;
}


bool TrackerCamera::init(const char* cameraInitFileName, const char* trackerInitFileName){
	//loading calibration data location from InitFile
	//camProxy->camera->loadCalibrationData("ps3eye_intrinsics_blu.xml");
	camProxy = new CameraRemoteProxy();
	bool initialized = true;
		if (!camProxy->camera->loadCalibrationData("ps3eye_intrinsics_blu.xml")){
			cout << "Camera calibration failed!!" << endl;
			initialized = false;
		}
	
	//loading tracker configFile from initFile
	TrackerConfigManager config_tracker(trackerInitFileName);
	
	
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

void TrackerCamera::startTracking(){
	running = true;
	cv::Mat image(480, 640, CV_8UC3);
	while (running)
	{
		bool calibrated = 0;

		while (!camProxy->CaptureAndTryCalibration(1));
		calibrated = true;
		imshow("chessboard", *camProxy->lastImageTaken);

		char ch = cv::waitKey(0);

		if (!calibrated)
			cout << "Camera is not calibrated!" << endl;
		for (int i = 0;; i++)
		{
			camProxy->CaptureImage();
			tracker->processFrame(*camProxy->lastImageTaken);
			char ch = cv::waitKey(25);
			if (ch == 27)
			{
				break;
			}
		}

		running = false;

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
