/**
 * @file Path.h
 */

#ifndef _PATH_H_
#define _PATH_H_

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
#include <cvrConfig/ConfigManager.h>

//std libraries
#include <string>
#include <vector>

#include <iostream>
#include <sstream>

//project files
#include "PathData.h"
#include "Controller/OutboundPackets/TrialSetup.h"

using namespace std;
using namespace osg;
using namespace cvr;
 
/**
 * Data Structure used for data recording of path subject takes during a trial.  Contains all data related to a single trial of the WaterMaze application.
 */
class Path
{
    public:
		/**
		 * Default Constructor
		 */
        Path();
        /**
		 * @brief This is the preferred constructor implemented in this project.
		 * @param ts Pointer to the TrialSetup object used in this trial.
		 */
        Path(TrialSetup* ts);
        /**
         * Destructor
         */
        virtual ~Path();
        
        //modifier methods
        /**
         * @param timeElapsed The amount of time that has transpired since the beginning of this trial.
         * @param pd Pointer to the PathData object being inserted. 
         * 
         * Used to add data to the internal data structure.
         */
        void addStep(float timeElapsed, PathData* pd);
        void succeeded();
        void reclaimMemory();
        
        //access methods
        /**
		 * Returns the total amount of time transpired during this trial.
         */
        float getTotalTime();
        /**
		 * Retrieves the last PathData object that was recorded.
         */
        PathData* getLatest();
        
        //data recording
        /**
         * @brief Creates the log file for this trial.
         * @param subjectID Unique identifier for the test subject.
         * @param paradigmID Unique identifier for the paradigm this trial belongs to.
         */
        void writeToLog(string subjectID, string paradigmID);
        /**
         * @param subjectID Unique identifier for the test subject.
         * @param paradigmID Unique identifier for the paradigm this trial belongs to.
         * 
         * Writes this trial to the info log file.
         */
        void writeTrialDataToLog(string subjectID, string paradigmID);
        /**
         * @param subjectID Unique identifier for the test subject.
         * @param paradigmID Unique identifier for the paradigm this trial belongs to. 
         * 
         * Writes this trial to the data log file.
         */
        void writeTrialInfoToLog(string subjectID, string paradigmID);
    protected:
		vector<PathData*> _path;	///< path data in chronological order.
		int _startingPos;
		int _finishPos;
		float _distance;
		PathData* _prevPathData;
		bool _success;
		TrialSetup* _setup;	///< data pertaining to the experimental setup of this trial.
};


#endif
