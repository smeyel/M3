#include "LaserPointerTracker.h"
#include "TrackerConfigManager.h"

using namespace cv;

	LaserPointerTracker::LaserPointerTracker() {
	detectionParameters = DetectionParameters();
	moreThanOnePoints = 0;
	noPoints = 0;
}
	
	void LaserPointerTracker::init(const char *configfilename){
		detectionParameters.loadParametersFrom(configfilename);
	}

/**
 * For testing!
 */
	void LaserPointerTracker::addTestWindows(Mat& filteredImage,
		vector<Vec3f>& circles, Mat& img) {

	namedWindow("circles", CV_WINDOW_AUTOSIZE);
	namedWindow("teszt", CV_WINDOW_AUTOSIZE);
	for (unsigned int i = 0; i < circles.size(); i++) {
		Point center(round(circles[i][0]), round(circles[i][1]));
		//radius a 3. parameter circles[0][2]
		circle(img, center, 200 / 32, Scalar(0, 0, 255), 2, 8);
	}

	imshow("teszt", filteredImage);
	imshow("circles", img);
}


	void LaserPointerTracker::addWindowsAndCounters(Mat& filtered, Mat& img)
	{
		circle(img, lastPoint, 200 / 32, Scalar(0, 255, 0), 2, 8);
		stringstream sout1, sout2;

		sout1 << "Frames with no laser point found: " << noPoints;
		sout2 << "Frames with more than one points found: " << moreThanOnePoints;

		putText(img, sout1.str(), Point(5, 25), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(0, 255, 0));
		putText(img, sout2.str(), Point(5, 50), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(0, 255, 0));

		imshow("img", img);
		imshow("bw", filtered);
	}


	void LaserPointerTracker::processFrame(Mat& img){
		/*
		//Jani:
		Mat filtered, hsvframe;
		//vector<Vec3f> circles;

		const Size dsize(img.rows, img.cols);

		cvtColor(img, hsvframe, CV_BGR2HSV);

		inRange(hsvframe, detectionParameters.minColorThreshold,
			detectionParameters.maxColorThreshold, filtered);

		GaussianBlur(filtered, filtered, Size(), 3, 3); // size 9,9 ; 2,2

		HoughCircles(filtered, lastFoundObjectCoords, CV_HOUGH_GRADIENT, 1, dsize.height / 4,
			detectionParameters.houghParam1, detectionParameters.houghParam2,
			detectionParameters.minRadius, detectionParameters.maxRadius);

		addTestWindows(filtered, lastFoundObjectCoords, img);
		
		//returns the coords and radius of the found circles.
		//return circles;
		*/

		//DZs's algorithm:
		Mat frameout;
		vector<Mat> channels;

		int color = detectionParameters.laserColor;

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
		addWeighted(channels[color], 1, averaging, -1, -detectionParameters.lowerTreshold, frameout);

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
			
			if ((area > detectionParameters.minArea) && (area < detectionParameters.maxArea)) 
			{
				double posX = mom.m10 / area;
				double posY = mom.m01 / area;

				newFoundPoints.push_back(Point(posX, posY));
			}
		}

		Point2i closest = closestPoint(lastPoint, newFoundPoints);
		Point2i mostIntense = largestIntensityPoint(newFoundPoints, channels[color]);

		
		lastPoint = newPoint(closest, mostIntense);
		
		addWindowsAndCounters(frameout, img);
	}





	//distance between two points:
	int LaserPointerTracker::sqDist(Point2i a, Point2i b)
	{
		return ((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
	}


	//Find the closest new point to the old one:
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
		if (closest == Point2i(0, 0))
		{
			if (mostIntense == Point2i(0, 0))
			{
				noPoints++;
				return  Point2i(0, 0);
			}
			else
				return mostIntense;
		}
		else
		if (closest != mostIntense)
		{
			moreThanOnePoints++;
			return Point2i(0, 0);
		}
		else
			return mostIntense;
	}

	LaserPointerTracker::~LaserPointerTracker() {
}

