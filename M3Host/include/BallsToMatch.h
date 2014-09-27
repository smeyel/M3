#ifndef __BALLSTOMATCH_H
#define __BALLSTOMATCH_H

#include "ObjectsToMatch.h"
#include "Ball.h"
class BallsToMatch : public ObjectsToMatch
{
public:
	vector<Ball>* BallData;
	BallsToMatch();
	~BallsToMatch();
};

#endif
