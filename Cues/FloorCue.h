/**
 * @file FloorCue.h
 */

#ifndef _FLOORCUE_H_
#define _FLOORCUE_H_

//OpenSceneGraph
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

//std includes
#include <iostream>
#include <string>

//project files
#include "../Cue.h"

using namespace std;
using namespace cvr;
using namespace osg;

/**
 * Subclass of Cue indicates different kinds of cues relating to the floor (red tiles, green circles, floor geometry)
 */
class FloorCue: public Cue
{
	public:
		/**
		* @param file  XML configuration file containing all the data necessary to complete this cue
		* 
		* Constructor
		*/
		FloorCue(string file);
		/**
		* Destructor
		*/
		virtual ~FloorCue();

		/**
		* @param _geoRoot MatrixTransform of scene space.
		* @param ts Setup of the currently running trial (or trial about to run).
		* 
		* Used to add geometry to the scene corresponding to this cue.
		*/
		void renderGeo(osg::ref_ptr<osg::MatrixTransform> _geoRoot, TrialSetup* ts);
		
		/**
		* Whether or not there will be an indicator for whether or not the test subject is not standing on the destination tile.
		*/
		bool isGrid();
		/**
		* Returns whether or not the destination tile is a circle.
		* 
		* 0: N/A
		* 1: yes 
		* 2: no 
		*/
		int isTileCircle();
	protected:
		bool _showGrid;
		string _subType;
		bool _circle;
		
};

#endif
