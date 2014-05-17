#ifndef __BALLTRACKER_H
#define __BALLTRACKER_H

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/opencv.hpp>

#include "ObjectTracker.h"
#include "Ball.h"
#include "BallTrackerConfigManager.h"

using namespace cv;
using namespace std;

/*	You can track many ping pong balls with this class
	Finds the contours of an object (defined by HSV min and max values)
	Fit circle on found objects, and matches with previously tracked balls
	Draws the road of each ball on each frame
	Searches for Collisions
	!!TODO: Solving Collisions
	*/

class BallTracker : public ObjectTracker{
	// Stores contours for each object
	vector<vector<Point2i>> Contours;
	vector<Point2i> ContourCenter;
	vector<int> ContourRadius;
	// Stores the indexes of balls which collided (smaller index, bigger index) 
	vector<Point2i> Collisions;
	// Stores the coords of each Collision
	Vector<Point2i> CollisionsPlace;
	/* Stores informations about tracked balls
	-Coords and radius from each frame, where it was detected (or predicted)
	-Whether it is visible now (or ran out of the frames)
	-For how many frames it cannot be detected
	*/
	vector<Ball> Balls;
	//Detection Parameterts
	Scalar HSVlow;
	Scalar HSVhigh;
	double CollidedBallsAreaRate;
	int MinSquareDistanceToCreateNewBall;
	int FramesNeededToDetectCollision;
	int FramesNeededToDropBall;
	int ContourMinSize;
	int SquareDistanceToInvolveCollision;
	int ErosionSize;
	int UseErosion;
	int UsePrediction;

public:
	// Constructor
	BallTracker();

	/*  Main processing function, only calls other functions
		Find and track balls in img
	*/
	void processFrame(Mat& img);
	// init
	void init(const char *configfilename);
	/*  Makes binary image with HSV filtering between Low and High values.
		Find only TOP level contours of each object (preventing "circle in circle") and stores
		@param img: binary image
		@param Low: minimum values for HSV filter
		@param High: maximum values for HSV filter
	*/
	void FindBallContoursUsingHSV(Mat& img);
	void CalculateContourParams();
	/*  Matches contours with previously tracked balls. If it can't find, creates new Ball
		For match, it uses the last coords of each tracked ball,
		or uses first order predict.
		@param fortesting: original picture so it can draw on it
		@param UsePredict: false->last coords ; true->prediction
	*/
	void MatchBallsWithContours(Mat &fortesting);
	void MatchContoursWithBalls(Mat &fortesting);
	/*  Calculates the square of the distance of to coords
		@param First,Second: the two points
	*/
	int SquareDistance(Point2i &First, Point2i &Second);
	/*	Finds the nearest tracked Ball to the currently find ball, used by MatchContoursWithBalls and CollisionDetection
		For searching, it uses the last coords of each tracked ball,
		or uses first order predict.
		@param NewBall: Found ball on the newest frame
		@param UsePredict: false>last coords ; true->prediction
	*/
	int FindClosestVisibleBall(Point2i &NewBall);
	int FindClosestContour(int BallID);
	/*	Finds the second nearest tracked Ball to the currently find ball, used by CollisionDetection
		For searching, it uses the last coords of each tracked ball,
		or uses first order predict.
		@param NewBall: Found ball on the newest frame
		@param UsePredict: false>last coords ; true->prediction
	*/
	int FindSecondClosestVisileBall(Point2i &NewBall);
	/*  Searches for Collision, where two tracked ball merges
		@param center: searches here
		@param radius: uses for decide whether it's a collision
		@param ClosestVisibleBall: marks Collision
	*/
	bool CollisionDetection(Point2i &center, int &radius, int &ClosestVisibleBall, int ContourIndex, Mat& fortesting);
	/*  Draws the travelled road for each visible Ball
		@param img: draws here
	*/
	void DrawVisibleBallRoutes(Mat &img);
	void ErodeFrame(Mat &img);

	Point2f getLastPoint();

	~BallTracker();
};

#endif