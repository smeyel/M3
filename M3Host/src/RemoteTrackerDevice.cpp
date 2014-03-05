#include "RemoteTrackerDevice.h"


RemoteTrackerDevice::RemoteTrackerDevice()
{
}

bool RemoteTrackerDevice::init(const char* initFile){
	//loading calibration data location from InitFile
	//camProxy->camera->loadCalibrationData("ps3eye_intrinsics_blu.xml");
	bool initialized = true;

	//test
	trackCams.push_back(new TrackerCamera());
	trackCams[0]->init("remote.ini", "tracker.ini");
	
	return initialized;
}


void RemoteTrackerDevice::connectToAllCamera(){
	for (int i = 0; i < trackCams.size(); i++){
		trackCams[i]->connect();
	}
}

void RemoteTrackerDevice::disconnectFromAllCamera(){
	
	cout << "Disconnecting..." << endl;
	for (int i = 0; i < trackCams.size(); i++){
		trackCams[i]->disconnect();
	}
	cout << "Done..." << endl;
}

void RemoteTrackerDevice::startTracking(){
	//ezt majd ha több kamerával próbáljuk, akkor szálkezeléssel kell megoldani!
	for (int i = 0; i < trackCams.size(); i++){
		trackCams[i]->startTracking();
	}
}
RemoteTrackerDevice::~RemoteTrackerDevice()
{
	for (int i = 0; i < trackCams.size(); i++){
		if (trackCams[i] != NULL){
			delete trackCams[i]; 
			trackCams[i] = NULL;
		}
	}
}

