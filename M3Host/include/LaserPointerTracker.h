/*
 * circleDetection.h
 *
 *  Created on: Feb 17, 2014
 *      Author: john
 */

#ifndef LASERPOINTERTRACKER_H_
#define LASERPOINTERTRACKER_H_

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/opencv.hpp>


#include "ObjectTracker.h"
#include "DetectionParameters.h"

using namespace cv;
using namespace std;

class LaserPointerTracker : public ObjectTracker{
	//Mat img;
	DetectionParameters detectionParameters;
	vector<Vec3f> lastFoundObjectCoords;

public:
	LaserPointerTracker();

	//ezeket be kell rakni az initbe és nem használni a balltype-ot és a colort
	LaserPointerTracker(BallType ballType, BallColor ballcolor);
	LaserPointerTracker(BallType ballType, BallColor ballColor, const char* filename);

	 void init(const char *configfilename);
	/**
	 * For test purpose it shows the original video frame and the filtered pictures.
	 */
	void addTestWindows(Mat& filteredImage, vector<Vec3f>& circles,Mat& img );
	
	/**
	* Detects circles on image .
	*/
	void processFrame(Mat& img);

	virtual ~LaserPointerTracker();
};

#endif /* LASERPOINTERTRACKER_H_ */
