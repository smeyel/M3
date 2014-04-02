#include "LaserScanner.h"


LaserScanner::LaserScanner()
{
}

bool LaserScanner::init(const char* initFile){
	bool initialized = true;
	trackCams.push_back(new TrackerCamera());
	trackCams.push_back(new TrackerCamera());
	trackCams[0]->init("remote.ini", "tracker.ini");
	trackCams[1]->init("remote2.ini", "tracker.ini");
	this->meshFile.open("meshLab.asc", ios::app);
	return initialized;
}

void LaserScanner::make3DCoords(){
	vector <Ray> rays;
	unsigned int valid3DPointCounter = 0;
	for (unsigned int i = 0; i < trackCams.size(); i++){
		rays.push_back(getRayToPointFromCam(i));
		if (rays[i].originalImageLocation != Point2f(0, 0))
		{
			valid3DPointCounter++;
		}
	}

	if (valid3DPointCounter >= 2)
	{
	
		export3DCoordsToFile(meshFile, Ray::getIntersection(rays, 0));
	}
}

Ray LaserScanner::getRayToPointFromCam(int camID){
	return trackCams[camID]->camProxy->camera->pointImg2World(trackCams[camID]->tracker->getLastPoint());
}

void LaserScanner::export3DCoordsToFile(ofstream& meshFile, Matx41f coord){

	meshFile << coord(0) << "," << coord(1) << "," << coord(2) << "," << coord(3) << endl;

}

void LaserScanner::startTracking(){
	
	while (1){
	
		for (unsigned int i = 0; i < trackCams.size(); i++){
			trackCams[i]->processFrame();
		}

		make3DCoords();
		
		
		char ch = cv::waitKey(25);
		if (ch == 27)
		{
			break;
		}
	}
}

LaserScanner::~LaserScanner()
{
	meshFile.close();
}
