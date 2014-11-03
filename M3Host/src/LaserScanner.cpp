#include "LaserScanner.h"




LaserScanner::LaserScanner()
{
#ifdef MULTITHREAD_MODE
  InitializeCriticalSection(&crit);

  Viewer = new PointCloudViewer(&crit, &FoundCoordinates);

  unsigned uiThreadID;

  handle = (HANDLE)_beginthreadex(NULL,
    0,
    PointCloudViewer::ThreadViewerStaticStartup,
    Viewer,
    CREATE_SUSPENDED,
    &uiThreadID);

  if (handle == 0)
    printf("Failed to create thread\n");

  DWORD dwEcitCode;

  GetExitCodeThread(handle, &dwEcitCode);
  printf("Thread exit code = %u\n", dwEcitCode);



  ResumeThread(handle);   // Jaeschke's   // t1->Start();
#endif
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

#ifdef MULTITHREAD_MODE
    EnterCriticalSection(&crit);
    FoundCoordinates.push_back(Ray::getIntersection(rays, 0));
    LeaveCriticalSection(&crit);
#endif

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
	
		for (auto &it : trackCams){
			it->processFrame();
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
#ifdef MULTITHREAD_MODE
  WaitForSingleObject(handle, INFINITE);

  DWORD dwEcitCode;
  GetExitCodeThread(handle, &dwEcitCode);
  printf("thread 1 exited with code %u\n", dwEcitCode);


  // The handle returned by _beginthreadex() has to be closed
  // by the caller of _beginthreadex().

  CloseHandle(handle);

  delete Viewer;
  Viewer = NULL;
#endif
}
