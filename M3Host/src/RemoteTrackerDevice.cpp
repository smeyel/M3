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
	trackCams.push_back(new TrackerCamera());
	trackCams[1]->init("remote2.ini", "tracker2.ini");
	//TODO: Depends on config
	vector<ObjectsToMatch*> tempObjectsToMatch;
	tempObjectsToMatch.push_back(trackCams[0]->tracker->getObjectsToMatch());
	tempObjectsToMatch.push_back(trackCams[1]->tracker->getObjectsToMatch());
	pObjectMatcher = new BallMatcher(tempObjectsToMatch);
	return initialized;
}


void RemoteTrackerDevice::connectToAllCamera(){
	for (unsigned int i = 0; i < trackCams.size(); i++){
		trackCams[i]->connect();
	}
}

void RemoteTrackerDevice::calibrateAllCamera()
{
	for (unsigned int j = 0; j < 10; j++)
	{
		for (unsigned int i = 0; i < trackCams.size(); i++){
			if (trackCams[i]->calibrate())
			{
				cout << "Camera is calibrated" << endl;
			}
			else
			{
				cout << "Camera is NOT calibrated" << endl;
			}
		}
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
		if (waitKey(30) == 27)
		{
			break;
		}
		pObjectMatcher->MatchObjects();
	}
	for (unsigned int i = 0; i < trackCams.size(); i++)
	{
		if (trackCams[i]->saveToFile)
		{
			trackCams[i]->savingToFile(i);
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
	delete pObjectMatcher;
}

