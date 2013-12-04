#include "EyeData.h"

EyeData::EyeData(osg::Vec3 right, osg::Vec3 front, osg::Vec3 up)
{
	_right = right;
	_front = front;
	_up = up;
	
	/// @todo plug in Eye tracking code to get data that we want and store it.
}

EyeData::~EyeData()
{
	
}

Vec3 EyeData::getRight()
{
	return _right;
}

Vec3 EyeData::getFront()
{
	return _front;
}

Vec3 EyeData::getUp()
{
	return _up;
}
