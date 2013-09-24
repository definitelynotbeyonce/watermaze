#ifndef _WALLCUE_H_
#define _WALLCUE_H_

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
#include <cvrConfig/XMLReader.h>

//cvr includes

//std includes
#include <vector>
#include <iostream>
#include <string>

//user defined includes
//#include "../Cue.h"
#include "../Controller/TrialSetup.h"

class WallCue
{
	public:
		WallCue();
		virtual ~WallCue();
		void renderGeo(osg::ref_ptr<osg::MatrixTransform> _geoRoot, std::string configFile, TrialSetup* ts, osg::Texture2D* tex);
	protected:
		
};

#endif
