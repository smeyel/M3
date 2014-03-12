#include "LaserPointerTracker.h"
#include "TrackerConfigManager.h"

using namespace cv;

	LaserPointerTracker::LaserPointerTracker() {
	detectionParameters = DetectionParameters();
	blankFrameNum = 0;
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

		Mat cont = Mat::zeros(img.rows, img.cols, CV_8UC1);
		vector<vector<Point>> contours;
		
		

		findContours(frameout.clone(), contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

		vector<Vec3f> newFoundObjectCoords;

		//for each contour with suitable size, save the coordinates of the centre 
		for (int i = 0; i< contours.size(); i++)
		{
			Moments mom = moments(contours[i], true);
			double area = contourArea(contours[i]);
			
			if ((area > detectionParameters.minArea) && (area < detectionParameters.maxArea)) 
			{
				double posX = mom.m10 / area;
				double posY = mom.m01 / area;

				Vec3f pos(posX, posY, 0);

				newFoundObjectCoords.push_back(pos);

			}
		}

		findClosestPoint(lastFoundObjectCoords, newFoundObjectCoords);

		addTestWindows(frameout, lastFoundObjectCoords, img);
	}


	//distance between two points:
	double LaserPointerTracker::dist(Point3f a, Point3f b)
	{
		return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y) + (a.z - b.z)*(a.z - b.z));
	}


	//Find the closest new point to the old one:
	void LaserPointerTracker::findClosestPoint(vector<Vec3f>& old, vector<Vec3f>& new_points)
	{
		
		if (old.empty())
		{
			if (!new_points.empty())
				old.push_back(new_points[0]);
		}
		else
		{
			if (new_points.empty())
					old.clear();
			else
			{
				int idx = 0;
				for (int i = 0; i < new_points.size(); i++)
				{
					if (dist(old[0], new_points[idx]) > dist(old[0], new_points[i]))
						idx = i;
				}
				old.front() = new_points[idx];
			}		 
		}
	}

	LaserPointerTracker::~LaserPointerTracker() {
}

