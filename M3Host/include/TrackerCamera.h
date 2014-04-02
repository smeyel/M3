#include "BallTracker.h"
#include "CameraProxy.h"
#include "ObjectTracker.h"
#include "CameraRemoteProxy.h"
#include "LaserPointerTracker.h"
#include <fstream>
#include "ray.h"


class TrackerCamera
{
public:
	/*
	Camera implementation.
	*/
	CameraRemoteProxy* camProxy;

	/*
	The tracker we use to process frames.
	*/
	ObjectTracker* tracker;

	/*
	Configuration manager for the tracker.
	*/
	TrackerConfigManager config_tracker;
	
	/*
	It is for saving frames into file.
	*/
	VideoWriter oVideoWriter;

	/*
	True if the tracking is in progress.
	*/
	bool running;

	string destination;

	/*
	True if we want to save the frames into a file.
	*/
	bool saveToFile;
	/*
	Tells if the camera is calibrated.
	*/
	bool isCalibrated;

	/*
	Used in processing to know if we initialized already.
	*/
	bool needinit;

	TrackerCamera();
	~TrackerCamera();

	/*
	Initializing the tracker.
	*/
	bool init(const char* cameraInitFileName, const char* trackerInitFileName);
	
	/*
	Connecting to a camera.
	*/
	void connect();

	/*
	Disconnenting from camera.
	*/
	void disconnect();

	/*
	Asking for an image and processing it.
	*/
	void processFrame();

private:
	/*
	Camera port to connect to.
	*/
	int port;

	/*
	Camera ip address to connect to.
	*/
	std::string ip_addr;
};

