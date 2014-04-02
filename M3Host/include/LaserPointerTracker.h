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
#include "LaserConfigManager.h"

#include <fstream>

using namespace cv;
using namespace std;

class LaserPointerTracker : public ObjectTracker{
	LaserConfigManager configManager;
	Point2i lastPoint;
	Mat averaging;
	Point2f getLastPoint();
	int failCntNoPoints;
	int failCntUndecideable;
	int failCntTooManyPoints;

	bool tooManyPoints;



public:
	LaserPointerTracker();

	/*
	Tracker initialization.
	*/
	void init(const char *configfilename);

	/**
	 * For test purpose it shows the original video frame and the filtered pictures.
	 */
	void addWindowsAndCounters(Mat& filtered, Mat& img);

	/**
	* Implements the point finder algorythm.
	*/
	void processFrame(Mat& img);

	/*
	Finds the closest point from <new_points> to the last found point.
	*/
	Point2i closestPoint(Point2i old, vector<Point2i> new_points);

	/*
	Finds and returns with the largest intensity point on the given image.
	*/
	Point2i largestIntensityPoint(vector<Point2i> points, Mat img);

	/*
	Returns with the new point if it is the closest and the most intense.
	*/
	Point2i newPoint(Point2i closest, Point2i mostIntense);
	
	/*
	Returns with the distance between two points.
	*/
	int sqDist(Point2i a, Point2i b);

	virtual ~LaserPointerTracker();
};

#endif /* LASERPOINTERTRACKER_H_ */
