#include "LaserPointerTracker.h"
#include "TrackerConfigManager.h"

using namespace cv;

	LaserPointerTracker::LaserPointerTracker() {
	failCntUndecideable = 0;
	failCntNoPoints = 0;
	failCntTooManyPoints = 0;

	tooManyPoints = false;
}
	ObjectsToMatch* LaserPointerTracker::getObjectsToMatch()
	{
		return NULL;
	}
	
	void LaserPointerTracker::init(const char *configfilename){
		configManager.readconfig(configfilename);
	}

	void LaserPointerTracker::addWindowsAndCounters(Mat& filtered, Mat& img)
	{
		circle(img, lastPoint, 200 / 32, Scalar(0, 0, 255), 2, 8);
		stringstream sout1, sout2, sout3;

		sout1 << "Frames with no laser point found: " << failCntNoPoints;
		sout2 << "Frames with undecideable laser point: " << failCntUndecideable;
		sout3 << "Frames with moving background: " << failCntTooManyPoints;

		putText(img, sout1.str(), Point(5, 25), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(0, 255, 0));
		putText(img, sout2.str(), Point(5, 50), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(0, 255, 0));
		putText(img, sout3.str(), Point(5, 75), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(0, 255, 0));

		//TODO: only the last camera's pictures shows, becouse the windows have the same name. Should use cameraID or else.
		imshow("img", img);
		imshow("bw", filtered);
	}


	void LaserPointerTracker::processFrame(Mat& img){
		
		Mat frameout;
		vector<Mat> channels;

		int color = configManager.laserColor;

		if (!(averaging.data))
		{
			split(img, channels);
			averaging = channels[color];
			return;
		}
	
		split(img, channels);
		blur(channels[color], channels[color], Size(2, 2));

		
		//weighted averaging: avg = 0.9*old_avg + 0.1*new
		addWeighted(averaging, 0.9, channels[color], 0.1, 0, averaging);
	
		//background subtraction + tresholding
		//addWeighted(channels[color], 1, averaging, -1, -detectionParameters.lowerTreshold, frameout);
		addWeighted(channels[color], 1, averaging, -1, -configManager.lowerTreshold, frameout);

		//increase contrast
		frameout.convertTo(frameout, -1, 100, 0);

		
		vector<vector<Point>> contours;

		findContours(frameout.clone(), contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

		vector<Point2i> newFoundPoints;

		//for each contour with suitable size, save the coordinates of the centre 
		for (int i = 0; i< contours.size(); i++)
		{
			Moments mom = moments(contours[i], true);
			double area = contourArea(contours[i]);

			if ((area > configManager.minArea) && (area < configManager.maxArea))
			{
				double posX = mom.m10 / area;
				double posY = mom.m01 / area;

				newFoundPoints.push_back(Point(posX, posY));
			}
		}

		tooManyPoints = false;

		if (newFoundPoints.size() > 4)
		{
			tooManyPoints = true;
			failCntTooManyPoints++;
		}
		Point2i closest = closestPoint(lastPoint, newFoundPoints);
		Point2i mostIntense = largestIntensityPoint(newFoundPoints, channels[color]);

		
			lastPoint = newPoint(closest, mostIntense);
		
		addWindowsAndCounters(frameout, img);
	};

	Point2f LaserPointerTracker::getLastPoint(){
		return  (Point2f)lastPoint;
	}





	/*
	Returns with the distance between two points.
	*/
	int LaserPointerTracker::sqDist(Point2i a, Point2i b)
	{
		return ((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
	}



	/*
	Finds the closest point from <new_points> to the last found point.
	*/
	Point2i LaserPointerTracker::closestPoint(Point2i last, vector<Point2i> new_points)
	{
		
		if (last == Point2i(0,0))
			return Point2i(0, 0);
		else
		{
			if (new_points.empty())
				return Point2i(0, 0);
			else
			{
				int idx = 0;
				for (int i = 0; i < new_points.size(); i++)
				{
					if (sqDist(last, new_points[idx]) > sqDist(last, new_points[i]))
						idx = i;
				}
				return new_points[idx];
			}		 
		}
	}

	/*
	Finds and returns with the largest intensity point on the given image.
	*/
	Point2i LaserPointerTracker::largestIntensityPoint(vector<Point2i> points, Mat img)
	{
		int max = 0;
		int idx=0;
		if (!points.size())
			return Point2i();
		for (int i = 0; i < points.size(); i++)
		{
			int intensity = img.at<uchar>(points[i].y, points[i].x);
			idx = (intensity < max) ? idx : i;
			max = (intensity < max) ? max : intensity;
		}
		return points[idx];
	}


	Point2i LaserPointerTracker::newPoint(Point2i closest, Point2i mostIntense)
	{
		if (tooManyPoints)
			return Point2i(0, 0);

		if (closest == Point2i(0, 0))
		{
			if (mostIntense == Point2i(0, 0))
			{
				failCntNoPoints++;
				return  Point2i(0, 0);
			}
			else
				return mostIntense;
		}
		else
		if (closest != mostIntense)
		{
			failCntUndecideable++;
			return Point2i(0, 0);
		}
		else
			return mostIntense;
	}



	LaserPointerTracker::~LaserPointerTracker() {
}

