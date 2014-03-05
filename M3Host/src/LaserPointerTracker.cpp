#include "LaserPointerTracker.h"
#include "TrackerConfigManager.h"

using namespace cv;

	LaserPointerTracker::LaserPointerTracker() {
	detectionParameters = DetectionParameters();
}
	LaserPointerTracker::LaserPointerTracker(BallType ballType, BallColor ballColor) {
	detectionParameters = DetectionParameters(ballType, ballColor);
}
	void LaserPointerTracker::init(const char *configfilename){
		TrackerConfigManager config(configfilename);
		detectionParameters.loadParameters(config.trackerType, config.trackerColor);
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
		circle(img, center, 200 / 32, Scalar(0, 0, 255), -1, 8);
	}

	imshow("teszt", filteredImage);
	imshow("circles", img);
}

	void LaserPointerTracker::processFrame(Mat& img){
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
	}

	

	LaserPointerTracker::~LaserPointerTracker() {
}

