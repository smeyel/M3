#ifndef __BALL_H
#define __BALL_H

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/core.hpp>

#include <vector>

using namespace cv;
using namespace std;

/*	This class tracks the road of a Ping Pong Ball	
	Used by the BallTracker class
*/

class Ball
{
	/*	Stores data for a tracked ball in each frame
		Format: (x coord, y coord, radius)
	*/
	vector<Point3i> BallData;
	/*  Stores visibility of ball
		true-> It's on the frame, false-> it ran out
	*/
	bool visible;
	/*	Stores for how many frames the ball cannot be seen
	*/
	int LastSeen;
	bool updated;

	KalmanFilter KF;
public:
	//contstructors
	Ball();
	Ball(int x, int y, int z);
	/*	Draws the travelled road for a Ball
		@param img: draws heere
	*/
	void DrawLine(Mat &img);
	/*	Returns the size of the balls road
	*/
	int GetSize();
	/*	Returns currently position
	*/
	Point2i GetPosition();
	/*	Returns currently radius
	*/
	int GetRadius();
	/*	Stores new point for the road
	*/
	void UpdateData(int x, int y, int r);
	/*	Predicts next position
		First order prediction
	*/
	Point2i PredictPosition();
	/*	Sets the visibility
	*/
	void SetVisible(bool isVisible);
	/*	Returns visibility
	*/
	bool GetVisible();
	/*	Increments LastSeen
	*/
	void IncrementLastSeen();
	/*	Decrement LastSeen
	*/
	void DecrementLastSeen();
	/*	Returns LastSeen
	*/
	int GetLastSeen();
	void DeInit();
	void SetUpdated(bool updated);
	bool GetUpdated();
	//destructor
	~Ball();
};
#endif