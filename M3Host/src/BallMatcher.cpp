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

void BallMatcher::MatchObjects(vector<Mat*> images,vector<string*> names)
{
	for (int i = 0; i < images.size(); i++)
	{
		circle(*images[i], Point(100, 100), 20, Scalar(255, 255, 0),-1);
		imshow(*names[i], *images[i]);
		waitKey(30);
	}
}