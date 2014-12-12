#include "BallMatcher.h"
#include "BallsToMatch.h"
#include "camera.h"

BallMatcher::BallMatcher(vector<Camera*> cameras, vector<ObjectsToMatch*> objectsToMatch, vector<Mat*> images, vector<string*> names) : ObjectMatcher(cameras, objectsToMatch, images, names)
{
	for (int i = 0; i < objectsToMatch.size(); i++)
	{
		detectedBalls.push_back(((BallsToMatch*)(objectsToMatch[i]))->BallData);
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


int BallMatcher::SquareDistance(Point2i& First, Point2i& Second)
{
//	cout << (First.x - Second.x)*(First.x - Second.x) + (First.y - Second.y)*(First.y - Second.y) << endl;
	return (First.x - Second.x)*(First.x - Second.x) + (First.y - Second.y)*(First.y - Second.y);
}

bool sortDistances(Point3i ball1, Point3i ball2)
{
	return ball1.z < ball2.z;
}

void BallMatcher::MatchBalls()
{
	vector < Point3i> distances;
	vector<Point2i> usedBalls;
	for (map<int, Point3i>::iterator it0 = detectedBalls[0]->begin(); it0 != detectedBalls[0]->end(); it0++)
	{
		
		for (map<int, Point3i>::iterator it1 = detectedBalls[1]->begin(); it1 != detectedBalls[1]->end(); it1++)
		{
			Ray ray0 = cameras[0]->pointImg2World(Point2f(it0->second.x, it0->second.y));
			Ray ray1 = cameras[1]->pointImg2World(Point2f(it1->second.x, it1->second.y));
			vector<Ray> rays;
			rays.push_back(ray0);
			rays.push_back(ray1);
			Matx41f worldPoint = Ray::getIntersection(rays, 0);
			Point2i ball0 = Point2i((cameras[0]->pointWorld2Img(worldPoint)));
			Point2i ball1 = Point2i((cameras[1]->pointWorld2Img(worldPoint)));
		//	cout << it0->first << " - " << it1->first << " world: " << worldPoint(0) << " " << worldPoint(1) << " " << worldPoint(2);
			distances.push_back(Point3i(it0->first, it1->first, SquareDistance(ball0, Point2i(it0->second.x, it0->second.y)) + SquareDistance(ball1, Point2i(it1->second.x, it1->second.y))));
			
		}

	}
	sort(distances.begin(), distances.end(), sortDistances);
	for (int i = 0; i < distances.size(); i++)
	{
		bool used = false;
		for (int j = 0; j < usedBalls.size(); j++)
		{
			if (distances[i].x == usedBalls[j].x || distances[i].y == usedBalls[j].y)
			{
				used = true;
				break;
			}
		}
		if (used == false)
		{
			pairedBalls.emplace(distances[i].x, distances[i].y);
			usedBalls.push_back(Point2i(distances[i].x, distances[i].y));
		}
	}

	//Another matcher implementation in visibility order

	/*for (map<int, Point3i>::iterator it0 = detectedBalls[0]->begin(), it1 = detectedBalls[1]->begin(); it0 != detectedBalls[0]->end(); it0++)
	{
		if(it1 != detectedBalls[1]->end())
		{
			pairedBalls.emplace(it0->first, it1->first);
			it1++;
		}
	}*/
}

void BallMatcher::DrawOnImages()
{
	int i = 0;
	for (map<int, Point3i>::iterator it0 = detectedBalls[0]->begin(); it0 != detectedBalls[0]->end(); it0++,i++)
	{
		if (pairedBalls.find(it0->first) != pairedBalls.end())
		{
			Ray ray0 = cameras[0]->pointImg2World(Point2f(it0->second.x, it0->second.y));
			Ray ray1 = cameras[1]->pointImg2World(Point2f((*detectedBalls[1])[pairedBalls[it0->first]].x, (*detectedBalls[1])[pairedBalls[it0->first]].y));
			vector<Ray> rays;
			rays.push_back(ray0);
			rays.push_back(ray1);
			Matx41f worldPoint = Ray::getIntersection(rays, 0);
			Point2f ball0(cameras[0]->pointWorld2Img(worldPoint));
			Point2f ball1(cameras[1]->pointWorld2Img(worldPoint));

			circle(*images[0], ball0, 10, Scalar((30 * i) % 256, (60 * i) % 256, (90 * i) % 256), -1);
			circle(*images[1], ball1, 10, Scalar((30 * i) % 256, (60 * i) % 256, (90 * i) % 256), -1);
		}
	}
	imshow(*names[0], *images[0]);
	imshow(*names[1], *images[1]);
	waitKey(30);
}
void BallMatcher::MatchObjects()
{
	pairedBalls.clear();
	MatchBalls();
	DrawOnImages();
}