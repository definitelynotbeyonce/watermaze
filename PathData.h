/**
 * @file PathData.h
 */

#ifndef _PATHDATA_H_
#define _PATHDATA_H_

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

//CalVR
#include <cvrKernel/PluginHelper.h>

//std libraries
#include <string>
#include <vector>
#include <map>

#include <iostream>

//project files
#include "EyeData.h"
#include "Controller/OutboundPackets/OutboundPacket.h"

using namespace std;

/**
 * Subclass of OutboundPacket contains all the data recorded at a single timestep.
 */
class PathData: public OutboundPacket
{
	public:
		/**
		 * @brief prefered constructor
		 * 
		 * Constructor: takes in the position vector of the head, 3 vectors necessary for calculating eye data, and the time since the beggining of the trial.
		 */
		PathData(osg::Vec3 pos, osg::Vec3 right, osg::Vec3 front, osg::Vec3 up, float elapsedTime);
		/**
		 * Destructor
		 */
		virtual ~PathData();
		
		//data recording
		/**
		 * @param outFile Data log file of the trial this piece of data belongs to.
		 * 
		 * Writes this piece of data to the log file for the trial it belongs to.
		 */
		void writeToLog(ofstream &outFile);
		
		//android reporting
		/**
		 * Used for android reporting.  Serializes this piece of data for network communication purposes.
		 */
		string getLine();
		
		//accessor methods
		/**
		 * Returns the position in scene space.
		 */
		osg::Vec3 getPos();
		/**
		 * Returns the time since the trial began.
		 */
		float getTime();
	protected:
		float _time;		///< time transpired since the beginning of the trial.
		osg::Vec3 _pos;		///< position of test subject in virtual space
		EyeData* _eyeData;	///< all data pertaining to the ocular focal data.
};



#endif
