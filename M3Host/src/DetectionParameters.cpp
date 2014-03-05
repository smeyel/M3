/*
 * BallType.cpp
 *
 *  Created on: Feb 17, 2014
 *      Author: john
 */

#include "DetectionParameters.h"

DetectionParameters::DetectionParameters() {
	minColorThreshold = CV_RGB(0,0,0);
	maxColorThreshold = CV_RGB(255,255,255);
	houghParam1 = 100;
	houghParam2 = 50;
	minRadius = 0;
	maxRadius = 500;
}

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

void DetectionParameters::loadParametersFrom(void* file){
	//not implemented
}

DetectionParameters::DetectionParameters(BallType ballType, BallColor color) {
	loadParameters(ballType,color);
}


DetectionParameters::~DetectionParameters() {
	// TODO Auto-generated destructor stub
}

