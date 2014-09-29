#include "ObjectMatcher.h"
#include "camera.h"

ObjectMatcher::ObjectMatcher(vector<Camera*> cameras, vector<ObjectsToMatch*> objectsToMatch, vector<Mat*> images, vector<string*> names)
{
	this->cameras = cameras;
	this->images = images;
	this->names = names;
}

ObjectMatcher::ObjectMatcher()
{

}

ObjectMatcher::~ObjectMatcher()
{

}