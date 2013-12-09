/**
 * @file WallCue.h
 */

#ifndef _WALLCUE_H_
#define _WALLCUE_H_

//OpenSceneGraph
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Material>
#include <osg/MatrixTransform>

#include <osg/Switch>
#include <osg/Texture2D>
#include <osgText/Text>
#include <osgDB/ReadFile>
#include <cvrConfig/XMLReader.h>
#include <cvrConfig/ConfigManager.h>

//std libraries
#include <vector>
#include <iostream>
#include <string>
#include <cmath>

//project files
#include "../Cue.h"


using namespace std;
using namespace osg;
using namespace cvr;


/**
 * Subclass of Cue indicating geometry relating to the walls
 */
class WallCue: public Cue
{
	public:
		/**
		* @param file XML configuration file containing all the data necessary to complete this cue
		* 
		* Constructor
		*/
		WallCue(string file);
		/**
		* Destructor
		*/
		virtual ~WallCue();
		
		/**
		* @param _geoRoot MatrixTransform of scene space.
		* @param ts Setup of the currently running trial (or trial about to run).
		* 
		* Used to add geometry to the scene corresponding to this cue.
		*/
		void renderGeo(osg::ref_ptr<osg::MatrixTransform> _geoRoot, TrialSetup* ts);
		/** 
		* Renders the geometry of the 4 walls.
		*/
		void renderWall(osg::ref_ptr<osg::MatrixTransform> _geoRoot, TrialSetup* ts);
		/**
		* Renders the geometry of a wall hanging.
		*/
		void renderHanging(osg::ref_ptr<osg::MatrixTransform> _geoRoot, TrialSetup* ts);
	protected:
		string _subType;
		
};

#endif
