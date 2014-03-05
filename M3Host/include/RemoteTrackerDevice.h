#include "CameraRemoteProxy.h"
#include "ObjectTracker.h"
#include "LaserPointerTracker.h"
#include "TrackerCamera.h"

class RemoteTrackerDevice
{
public:
	bool running;
	vector<TrackerCamera*> trackCams;
	RemoteTrackerDevice();
	~RemoteTrackerDevice();
	bool init(const char* initFile);

	void connectToAllCamera();
	void disconnectFromAllCamera();

	void startTracking();
};

