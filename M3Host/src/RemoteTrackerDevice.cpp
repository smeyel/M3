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
	vector<Mat*> images;
	images.push_back(trackCams[0]->camProxy->lastImageTaken);
	images.push_back(trackCams[1]->camProxy->lastImageTaken);
	vector<string*> names;
	names.push_back(&trackCams[0]->TrackerCameraName);
	names.push_back(&trackCams[1]->TrackerCameraName);
	pObjectMatcher = new BallMatcher(tempObjectsToMatch,images,names);
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
     //char fname[20];
     //sprintf_s(fname, 19, "Tmatrix_%d.xml", i);
     // bool iscalibrated = trackCams[i]->camProxy->camera->getIsTSet();
			if (trackCams[i]->calibrate())
			{
      //  if (!iscalibrated)
      //    trackCams[i]->camProxy->camera->saveExtrinsicParams(fname);
				cout << "Camera is calibrated" << endl;
			}
			else
			{
      //  if (!iscalibrated)
      //  {
      //    FileStorage fs(fname, FileStorage::READ);
      //    if (fs.isOpened())
      //    {
      //      fs.release();
      //      trackCams[i]->camProxy->camera->loadExtrinsicParams(fname);
      //    }
      //   else
      //     cout << "Camera is NOT calibrated" << endl;
      // }
      //
      // else
				  cout << "Camera is NOT calibrated" << endl;
			}
			trackCams[i]->VideoPuffer.push_back(trackCams[i]->camProxy->lastImageTaken->clone());
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
		pObjectMatcher->MatchObjects();
		for (unsigned int i = 0; i < trackCams.size(); i++){
			if (trackCams[i]->saveToFile){
				//Writes the frame into a puffer.
				trackCams[i]->VideoPuffer.push_back(trackCams[i]->camProxy->lastImageTaken->clone());
			}
		}
		if (waitKey(30) == 27)
		{
			break;
		}
	}
}

void RemoteTrackerDevice::saveToFile()
{

	//TODO: config dependent separated or merged video

	/*for (unsigned int i = 0; i < trackCams.size(); i++)
	{
		if (trackCams[i]->saveToFile)
		{
			vector<vector<Mat>*> temp;
			temp.push_back(&trackCams[i]->VideoPuffer);
			string tempDestination;
			tempDestination = "cam_";
			tempDestination += to_string(i);
			tempDestination += "_";
			tempDestination += trackCams[i]->destination;
			saveVideoPuffersToFile(tempDestination, temp);
		}
	}*/
	vector<vector<Mat>*> temp;
	for (unsigned int i = 0; i < trackCams.size(); i++)
	{
		if (trackCams[i]->saveToFile)
		{
			temp.push_back(&trackCams[i]->VideoPuffer);
		}
	}
	saveVideoPuffersToFile("MergedVideo.avi", temp);
}

void RemoteTrackerDevice::saveVideoPuffersToFile(string destiny, vector<vector<Mat>*> videoPuffers)
{
	int heightMax = 0;
	int widthMax = 0;
	int cameraCols = 0;
	int cameraRows = 0;
	if (videoPuffers.empty())
	{
		cout << "ERROR: Empty puffer!" << endl;
		return;
	}
	else
	{
		if (videoPuffers[0]->empty())
		{
			cout << "ERROR: Empty first video!" << endl;
			return;
		}
	}
	if (videoPuffers.size() == 1)
	{
		heightMax = (*videoPuffers[0])[0].rows;
		widthMax = (*videoPuffers[0])[0].cols;
		cameraCols = 1;
		cameraRows = 1;
	}
	else
	{
		for (unsigned int i = 0; i < videoPuffers.size(); i++)
		{
			if ((*videoPuffers[i])[0].rows > heightMax)
			{
				heightMax = (*videoPuffers[i])[0].rows;
			}
			if ((*videoPuffers[i])[0].cols > widthMax)
			{
				widthMax = (*videoPuffers[i])[0].cols;
			}
		}
		cameraCols = ceil(4.0*sqrt((double)heightMax*(double)videoPuffers.size() / (double)widthMax) / 3.0);
		cameraRows = ceil((((double)videoPuffers.size() - 0.001) / (double)cameraCols));
	}

	VideoWriter oVideoWriter;
	/* Save To File
	For help look http://opencv-srf.blogspot.hu/2011/09/saving-images-videos_16.html */
	cout << "Frame Size = " << cameraCols*widthMax << "x" << cameraRows*heightMax << endl;
	Size frameSize(cameraCols*widthMax,cameraRows*heightMax);
	oVideoWriter = VideoWriter(destiny, CV_FOURCC('M', 'P', '4', '2'), 20, frameSize, true); //initialize the VideoWriter object 00
	if (!oVideoWriter.isOpened()) //if not initialize the VideoWriter successfully, exit the program
	{
		cout << "ERROR: Failed to write the video" << endl;
		return;
	}

	for (unsigned imageNum = 0; imageNum < videoPuffers[0]->size(); imageNum++)
	{
		Mat combine = Mat::zeros(cameraRows*heightMax, cameraCols*widthMax, (*videoPuffers[0])[0].type());
		for (unsigned int i = 0; i < cameraRows; i++)
		{
			for (unsigned j = 0; j < cameraCols; j++)
			{
				Mat roi(combine, Rect(j*widthMax, i*heightMax, widthMax, heightMax));
				if ((i*cameraCols + j) < videoPuffers.size() && (imageNum<videoPuffers[i*cameraCols + j]->size()))
				{
					(*videoPuffers[i*cameraCols + j])[imageNum].copyTo(roi);
				}
				else break;;
			}
		}
		oVideoWriter.write(combine.clone());
	}
	oVideoWriter.release();
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

