/*
 * DetectionParameters.h
 *
 *  Created on: Feb 17, 2014
 *      Author: john
 */

#ifndef DETECTIONPARAMETERS_H_
#define DETECTIONPARAMETERS_H_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "VideoInput.h"

using namespace cv;
using namespace std;
enum BallType {
	LASER, PINGPONG
};
enum BallColor {
	RED, BLUE
};

class DetectionParameters {
public:
	Scalar minColorThreshold;
	Scalar maxColorThreshold;
	double houghParam1;
	double houghParam2;
	double minRadius;
	double maxRadius;
public:
	DetectionParameters();
	DetectionParameters(BallType ballType, BallColor color);
	void loadParameters(BallType ballType, BallColor color);
	void loadParametersFrom(void* file);
	virtual ~DetectionParameters();
};

#endif /* DETECTIONPARAMETERS_H_ */
