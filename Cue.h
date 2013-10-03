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
#include <cvrConfig/XMLReader.h>

#include <string>

#include "Controller/OutboundPackets/TrialSetup.h"

using namespace std;
using namespace cvr;

class Cue
{
	public:
		Cue(string file);
		virtual ~Cue();
		virtual void renderGeo(osg::ref_ptr<osg::MatrixTransform> _geoRoot, TrialSetup* ts) = 0;
		bool isRenderable();
		string toString();
		string getText();
		bool getState();
		string getType();
		void toggle(bool state);
	protected:
		string _type;
		string _text;
		string _cueFile;
		XMLReader _reader;
		bool _toggleState;
		bool _canRender;
};

#endif
