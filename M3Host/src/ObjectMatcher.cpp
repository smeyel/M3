#include "ObjectMatcher.h"

ObjectMatcher::ObjectMatcher(vector<ObjectsToMatch*> ObjectsToMatch, vector<Mat*> images, vector<string*> names)
{
	this->images = images;
	this->names = names;
}

ObjectMatcher::ObjectMatcher()
{

}

ObjectMatcher::~ObjectMatcher()
{

}