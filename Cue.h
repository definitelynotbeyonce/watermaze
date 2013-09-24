#ifndef _CUE_H_
#define _CUE_H_

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
#include <cvrKernel/PluginHelper.h>

using namespace std;

class Cue
{
	public:
		Cue();
		virtual ~Cue();
		void renderGeo(osg::ref_ptr<osg::MatrixTransform> _geoRoot) = 0;
	protected:
		
};

#endif
