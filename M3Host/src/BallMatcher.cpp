#include "BallMatcher.h"
#include "BallsToMatch.h"
#include "camera.h"

BallMatcher::BallMatcher(vector<Camera*> cameras, vector<ObjectsToMatch*> objectsToMatch, vector<Mat*> images, vector<string*> names) : ObjectMatcher(cameras, objectsToMatch, images, names)
{
	for (int i = 0; i < objectsToMatch.size(); i++)
	{
		BallsToMatchData.push_back(((BallsToMatch*)(objectsToMatch[i]))->BallData);
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

void BallMatcher::MatchBalls()
{
	for (int i = 0, j = 0; i < BallsToMatchData[0]->size(); i++)
	{
		if ((*BallsToMatchData[0])[i].GetVisible())
		{
			for (; j < BallsToMatchData[1]->size(); j++)
			{
				if ((*BallsToMatchData[1])[j].GetVisible())
				{
					vector<int> temp;
					temp.push_back(i);
					temp.push_back(j);
					PairIndexes.push_back(temp);
					j++;
					break;
				}
			}
		}
	}
}

void BallMatcher::DrawOnImages()
{
	for (int i = 0; i < PairIndexes.size(); i++)
	{
		circle(*images[0], Point((*BallsToMatchData[0])[PairIndexes[i][0]].GetPosition()), (*BallsToMatchData[0])[PairIndexes[i][0]].GetRadius(), Scalar((30 * i) % 256, (60 * i) % 256, (90 * i) % 256), -1);
		circle(*images[1], Point((*BallsToMatchData[1])[PairIndexes[i][1]].GetPosition()), (*BallsToMatchData[1])[PairIndexes[i][1]].GetRadius(), Scalar((30 * i) % 256, (60 * i) % 256, (90 * i) % 256), -1);
	}
	imshow(*names[0], *images[0]);
	imshow(*names[1], *images[1]);
	waitKey(30);
}
void BallMatcher::MatchObjects()
{
	LastPairIndexes = PairIndexes;
	PairIndexes.erase(PairIndexes.begin(), PairIndexes.end());
	MatchBalls();
	DrawOnImages();
}