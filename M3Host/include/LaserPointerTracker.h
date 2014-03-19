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
	
	Point2i lastPoint;
	Mat averaging;
	int failCntNoPoints;
	int failCntUndecideable;
	int failCntTooManyPoints;

	bool tooManyPoints;



public:
	LaserPointerTracker();

	//ezeket be kell rakni az initbe és nem használni a balltype-ot és a colort
	
	 void init(const char *configfilename);
	/**
	 * For test purpose it shows the original video frame and the filtered pictures.
	 */
	void addTestWindows(Mat& filteredImage, vector<Vec3f>& circles,Mat& img );

	void addWindowsAndCounters(Mat& filtered, Mat& img);
	/**
	* Detects circles on image .
	*/
	void processFrame(Mat& img);

	Point2i closestPoint(Point2i old, vector<Point2i> new_points);

	Point2i largestIntensityPoint(vector<Point2i> points, Mat img);

	Point2i newPoint(Point2i closest, Point2i mostIntense);

	
	int sqDist(Point2i a, Point2i b);

	virtual ~LaserPointerTracker();
};

#endif /* LASERPOINTERTRACKER_H_ */
