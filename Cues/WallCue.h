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
#include <cvrConfig/ConfigManager.h>

//cvr includes

//std includes
#include <vector>
#include <iostream>
#include <string>

//user defined includes
#include "../Cue.h"


class WallCue: public Cue
{
	public:
		WallCue(string file);
		virtual ~WallCue();
		void renderGeo(osg::ref_ptr<osg::MatrixTransform> _geoRoot, TrialSetup* ts);
	protected:
		
};

#endif
