#include "BallMatcher.h"
#include "BallsToMatch.h"

BallMatcher::BallMatcher(vector<ObjectsToMatch*> ObjectsToMatch)
{
	for (int i = 0; i < ObjectsToMatch.size(); i++)
	{
		BallsToMatchData.push_back(((BallsToMatch*)(ObjectsToMatch[i]))->BallData);
	}
}

BallMatcher::BallMatcher()
{

}

BallMatcher::~BallMatcher()
{

}

void BallMatcher::init(const char *configfilename)
{

}

void BallMatcher::MatchObjects()
{

}