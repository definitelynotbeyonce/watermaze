/**
 * @file Cue.h
 */

#ifndef _CUE_H_
#define _CUE_H_

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
#include <cvrKernel/PluginHelper.h>
#include <cvrConfig/XMLReader.h>

//std libraries
#include <string>

//project files
#include "Controller/OutboundPackets/TrialSetup.h"

using namespace std;
using namespace cvr;

/**
 * Class used to describe a single cue.  
 */
class Cue
{
	public:
		/**
		* @param file XML configuration file containing all the data necessary to complete this cue
		* 
		* Constructor
		*/
		Cue(string file);
		/**
		* Destructor
		*/
		virtual ~Cue();
		
	//Rendering functions
		/**
		* @param _geoRoot MatrixTransform of scene space.
		* @param ts Setup of the currently running trial (or trial about to run).
		* 
		* Pure virtual function used to add geometry to the scene corresponding to this cue.
		*/
		virtual void renderGeo(osg::ref_ptr<osg::MatrixTransform> _geoRoot, TrialSetup* ts) = 0;
		
	//Modifier methods
		/**
		* @param state New state of this cue
		* 
		* Used to change the toggle state of this cue.
		*/
		void toggle(bool state);
		
	//Access methods
		/**
		* Whether or not this cue has renderable geometry.
		*/
		bool isRenderable();
		/**
		* Returns the string representation of this cue.
		*/
		string toString();
		/**
		* Returns the unique identifier key pertaining to this cue.
		*/
		string getText();
		/**
		* Returns the toggle state of this cue.
		*/
		bool getState();
		/**
		* Returns the string representation of the cue type.
		*/
		string getType();
	protected:
		string _type;		///< type of the cue
		string _text;		///< unique identifier key
		string _cueFile;	
		XMLReader _reader;	
		bool _toggleState;	///< on/off state of cue
		bool _canRender;
};

#endif
