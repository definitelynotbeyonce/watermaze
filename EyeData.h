#ifndef _EYEDATA_H_
#define _EYEDATA_H_

//osg includes
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Material>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/ShapeDrawable>
#include <osg/Switch>
#include <osg/Texture2D>
#include <osgText/Text>
#include <osgDB/ReadFile>

//cvr includes

//std includes
#include <ctime>

//user defined includes

class EyeData
{
	public:
		EyeData();
		virtual ~EyeData();
	protected:
		osg::Vec3 _lookAt;
		
};

#endif
