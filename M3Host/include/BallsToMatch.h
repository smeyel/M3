#ifndef __BALLSTOMATCH_H
#define __BALLSTOMATCH_H

#include "ObjectsToMatch.h"
#include "Ball.h"
class BallsToMatch : public ObjectsToMatch
{
public:
	map<int,Point3i>* BallData;
	BallsToMatch();
	~BallsToMatch();
};

#endif
