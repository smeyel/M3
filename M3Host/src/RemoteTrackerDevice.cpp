#include "RemoteTrackerDevice.h"
#include <thread>
#include "ray.h"

using namespace std;

RemoteTrackerDevice::RemoteTrackerDevice()
{
}

bool RemoteTrackerDevice::init(const char* initFile){
	bool initialized = true;
	trackCams.push_back(new TrackerCamera());
	trackCams[0]->init("remote.ini", "tracker.ini");
	return initialized;
}


void RemoteTrackerDevice::connectToAllCamera(){
	for (unsigned int i = 0; i < trackCams.size(); i++){
		trackCams[i]->connect();
	}
}

void RemoteTrackerDevice::disconnectFromAllCamera(){
	
	cout << "Disconnecting..." << endl;
	for (unsigned int i = 0; i < trackCams.size(); i++){
		trackCams[i]->disconnect();
	}
	cout << "Done..." << endl;
}


void RemoteTrackerDevice::startTracking(){
	//TODO: only processing when there are other frames to get.
	while (1){
		for (unsigned int i = 0; i < trackCams.size(); i++){
			trackCams[i]->processFrame();
		}
	}
}


RemoteTrackerDevice::~RemoteTrackerDevice()
{
	for (unsigned int i = 0; i < trackCams.size(); i++){
		if (trackCams[i] != NULL){
			delete trackCams[i]; 
			trackCams[i] = NULL;
		}
	}
}

