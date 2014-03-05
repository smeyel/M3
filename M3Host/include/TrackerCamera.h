#include "BallTracker.h"
#include "CameraProxy.h"
#include "ObjectTracker.h"
#include "CameraRemoteProxy.h"
#include "LaserPointerTracker.h"

class TrackerCamera
{
public:
	CameraRemoteProxy* camProxy;
	ObjectTracker* tracker;
	bool running;

	//j� lenne ide egy config manager szer�s�g, mint ami a m�sik oldalon a MyPhoneProxyn�l van!
	TrackerCamera();
	bool init(const char* cameraInitFileName, const char* trackerInitFileName);
	void connect();
	void disconnect();
	//most csak teszt jelleggel vannak ezek itt:
	bool loadCalibrationData(const char* fileName);
	void loadTracker(ObjectTracker* tracker);
	////
	void startTracking();
	~TrackerCamera();
private:
	int port;
	std::string ip_addr;
};

