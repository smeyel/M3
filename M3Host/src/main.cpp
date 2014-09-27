#include <iostream>
//#include "opencv2/highgui.h"
#include <opencv2/opencv.hpp>
#define _WINSOCKAPI_
//#include "PhoneProxy.h"
#include "stdlib.h"
#include "StdoutLogger.h"
//#include "myconfigmanager.h"
#include "JpegMessage.h"
#include "LaserPointerTracker.h"
// Marker detection and tracking
#include "chessboarddetector.h"
#include "camera.h"
#include "PhoneProxy.h"
#include "RemoteTrackerDevice.h"
#include "LaserScanner.h"


using namespace std;

int main()
{
	 
	bool running = true;
	int frameIdx = 0;
	LogConfigTime::StdoutLogger logger;

	RemoteTrackerDevice scanner;
	scanner.init("scannerInitFileName");
	cout << "Connecting..." << endl;
	scanner.connectToAllCamera();
	scanner.calibrateAllCamera();
	scanner.startTracking();
	scanner.disconnectFromAllCamera();

	cout << "Done." << endl;
	return 0;
}
