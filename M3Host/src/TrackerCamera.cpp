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

	//save video to file
	saveToFile = config_tracker.saveToFile;
	destination = config_tracker.destination;

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

bool TrackerCamera::SaveVideoToFile(vector<Mat> &VideoPuffer)
{
	/* Save To File
	For help look http://opencv-srf.blogspot.hu/2011/09/saving-images-videos_16.html */
	double dWidth = 640; //get the width of frames of the video
	double dHeight = 480; //get the height of frames of the video
	cout << "Frame Size = " << dWidth << "x" << dHeight << endl;
	Size frameSize(static_cast<int>(dWidth), static_cast<int>(dHeight));
	VideoWriter oVideoWriter(destination, CV_FOURCC('M', 'P', '4', '2'), 20, frameSize, true); //initialize the VideoWriter object 00
	if (!oVideoWriter.isOpened()) //if not initialize the VideoWriter successfully, exit the program
	{
		cout << "ERROR: Failed to write the video" << endl;
		return false;
	}
	for (int j = 0; j < VideoPuffer.size(); j++)
	{
		oVideoWriter.write(VideoPuffer[j]); //writer the frame into the 
	}
	return true;
}

void TrackerCamera::startTracking(){
	running = true;
	cv::Mat image(480, 640, CV_8UC3);
	while (running)
	{
		bool calibrated = 0;

		vector<Mat> VideoPuffer;

		if (!calibrated)
			cout << "Camera is not calibrated!" << endl;
		cout << "Press ESC to exit" << endl;
		for (int i = 0;; i++)
		{
			camProxy->CaptureImage();
			imshow("labda", *camProxy->lastImageTaken);
			tracker->processFrame(*camProxy->lastImageTaken);
			if (saveToFile) VideoPuffer.push_back(camProxy->lastImageTaken->clone()); 
			char ch = cv::waitKey(25);
			if (ch == 27)
			{
				break;
			}
		}
		
		if (saveToFile)
		{
			SaveVideoToFile(VideoPuffer);
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
