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
    trackCams[i]->camProxy->chessboarddetector->chessboard.boardSize.width = 7;
    trackCams[i]->camProxy->chessboarddetector->chessboard.boardSize.height = 5;
	}
}

void RemoteTrackerDevice::calibrateAllCamera()
{
	for (unsigned int j = 0; j < 10; j++)
	{
		for (unsigned int i = 0; i < trackCams.size(); i++){
      char fname[20];
      sprintf_s(fname, 19, "Tmatrix_%d.xml", i);
      bool iscalibrated = trackCams[i]->camProxy->camera->getIsTSet();
			if (trackCams[i]->calibrate())
			{
        if (!iscalibrated)
          trackCams[i]->camProxy->camera->saveExtrinsicParams(fname);
				cout << "Camera is calibrated" << endl;
			}
			else
			{
        if (!iscalibrated)
        {
          FileStorage fs(fname, FileStorage::READ);
          if (fs.isOpened())
          {
            fs.release();
            trackCams[i]->camProxy->camera->loadExtrinsicParams(fname);
          }
          else
            cout << "Camera is NOT calibrated" << endl;
        }

        else
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
		vector<Mat*> images;
		vector<string*> names;
		for (unsigned int i = 0; i < trackCams.size(); i++){
			images.push_back(trackCams[i]->camProxy->lastImageTaken);
			names.push_back(&trackCams[i]->TrackerCameraName);
		}
		pObjectMatcher->MatchObjects(images,names);
		for (unsigned int i = 0; i < trackCams.size(); i++){
			if (trackCams[i]->saveToFile){
				//Writes the frame into a puffer.
				trackCams[i]->VideoPuffer.push_back(trackCams[i]->camProxy->lastImageTaken->clone());
			}
		}

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

