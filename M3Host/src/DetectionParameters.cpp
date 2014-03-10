/*
 * BallType.cpp
 *
 *  Created on: Feb 17, 2014
 *      Author: john
 */

#include "DetectionParameters.h"
#include "LaserConfigManager.h"

DetectionParameters::DetectionParameters() {
	minColorThreshold = CV_RGB(0,0,0);
	maxColorThreshold = CV_RGB(255,255,255);
	houghParam1 = 100;
	houghParam2 = 50;
	minRadius = 0;
	maxRadius = 500;

	laserColor = 1;
	lowerTreshold = 30;
	minArea = 0;
	maxArea = 500;
}
/*
void DetectionParameters::loadParameters(BallType ballType, BallColor color) {
	switch (ballType) {
	case LASER:
		houghParam1 = 30;
		houghParam2 = 10;
		minRadius = 2;
		maxRadius = 200;
		break;
	case PINGPONG:
		houghParam1 = 100;
		houghParam2 = 50;
		minRadius = 5;
		maxRadius = 400;
		break;
	}
	switch (color) {
	case BLUE:
		minColorThreshold = CV_RGB(80,0,0);//120,70,50 labda//80,0,0 laser
		//minColorThreshold = CV_RGB(100,50,30);
		maxColorThreshold = CV_RGB(255,255,255); //2550,211,150 labda// 255,255,255 laser
		break;
	case RED:
		minColorThreshold = CV_RGB(140,30,5);//40,100,220///  27. 4. 193. ////56. 46. 260
		maxColorThreshold = CV_RGB(255,210,30);//110,170,255
		break;
	}
}
*/
void DetectionParameters::loadParametersFrom(const char* file){
	LaserConfigManager config((const char*)file);

	houghParam1 = config.houghParam1;
	houghParam2 = config.houghParam2;
	
	minRadius = config.minRadius;
	maxRadius = config.maxRadius;

	minColorThreshold = CV_RGB(config.minColorThreshold_R, config.minColorThreshold_G, config.minColorThreshold_B);
	maxColorThreshold = CV_RGB(config.maxColorThreshold_R, config.maxColorThreshold_G, config.maxColorThreshold_B);

	laserColor = config.laserColor;
	lowerTreshold = config.lowerTreshold;
	minArea = config.minArea;
	maxArea = config.maxArea;

}

/*
DetectionParameters::DetectionParameters(BallType ballType, BallColor color) {
	loadParameters(ballType,color);
}
*/

DetectionParameters::~DetectionParameters() {
	// TODO Auto-generated destructor stub
}

