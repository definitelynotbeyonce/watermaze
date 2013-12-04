/**
 * @file WaterMaze.h
 */

#ifndef WATERMAZE_PLUGIN_H
#define WATERMAZE_PLUGIN_H

//OpenSceneGraph
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Material>
#include <osg/BlendColor>
#include <osg/BlendFunc>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/ShapeDrawable>
#include <osg/Switch>
#include <osg/Texture2D>
#include <osgText/Text>
#include <osgDB/ReadFile>

//CalVR
#include <cvrKernel/ComController.h>
#include <cvrKernel/CVRPlugin.h>
#include <cvrKernel/FileHandler.h>
#include <cvrKernel/InteractionManager.h>
#include <cvrKernel/PluginHelper.h>
#include <cvrKernel/SceneManager.h>
#include <cvrKernel/SceneObject.h>

#include <cvrConfig/ConfigManager.h>

#include <cvrMenu/SubMenu.h>
#include <cvrMenu/MenuRangeValue.h>
#include <cvrMenu/MenuCheckbox.h>
#include <cvrMenu/MenuButton.h>
#include <cvrMenu/MenuText.h>

#include <cvrUtil/CVRSocket.h>
#include <cvrUtil/MultiListenSocket.h>
#include <cvrUtil/Intersection.h>
#include <cvrUtil/OsgPrint.h>

//std libraries
#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <sstream>

//project files
#include "Path.h"
#include "Paradigm.h"

#include "Controller/WMController.h"
#include "Controller/OutboundPackets/TrialSetup.h"
#include "Controller/OutboundPackets/CueList.h"
#include "Controller/OutboundPackets/GeneralComm.h"

#include "Cues/WallCue.h"

namespace WaterMaze
{
	class WMController;
/**
 * Plugin Main.
 */
class WaterMaze: public cvr::CVRPlugin, public cvr::MenuCallback
{
    public:
		//CVRPlugin overrides
		/**
		 * Constructor
		 */
        WaterMaze();
        /**
		 * Destructor
		 */
        virtual ~WaterMaze();
        static WaterMaze * instance();
        /**
		 * Plugin initialization for CalVR.
		 */
        bool init();
        /**
		 * Called when a CalVR menu button is pressed.  Note: not used.
		 */
        void menuCallback(cvr::MenuItem * item);
        /**
		 * Called every rendering cycle.  Used for game control.
		 */
        void preFrame();
        /**
		 * Used to handle mouse click events and keyboard events.  Note: Keyboard interaction is coded for, but Plugin will not have full capabilites and is not guarangeed to work properly without an android controller.
		 */
        bool processEvent(cvr::InteractionEvent * event);
		
		//state change functions
		/**
		 * Load geometry into scene.
		 */
		void load();
		/**
		 * @param direction 1 next paradigm, -1 previous paradigm.  Note: Only prgress to next paradigm.
		 * 
		 * Used to change the current paradigm.
		 */
		void changeParadigm(int direction);
		/**
		 * @param direction Should always be 1.
		 * 
		 * Used to go to the next trial. 
		 */
		void changeTrial(int direction);
		/**
		 * Used to start a trial or for the operator to force quit a trial.
		 */
		void startStop();
		/**
		 * Override trial limit.
		 */
		void addTrial();
		
		//access methods
		/**
		 * Returns the string representation of the current running state.
		 */
		string getState();
		/**
		 * Returns the TrialSetup that is currently or about to be run.
		 */
		TrialSetup* getTrialSetup();
		/**
		 * Returns the CueList that is currently applicable.
		 */
		CueList* getCueList();
		
		/**
		 * @param action integer value of the action to be taken.
		 *
		 * Implementation in WaterMazeTakeAction.cpp
		 */
		void takeAction(int action);
		/**
		 * Sync a cue toggle across all nodes.
		 */
		void syncToggle();
		/**
		 * Sync a new subject being introduced across all nodes.
		 */
		void syncNewSubject();
		/**
		 * Logic to run the ending procedure of a trial.
		 */
		void handleEnding();
		/**
		 * Change the state after the ending procedure is finished.
		 */
		void endingChangeState();
		/**
		 * Logic to run the starting procedure of a trial.
		 */
		void handleStart();
		/**
		 * 
		 */
		void trialStartStateChange();
		/**
		 * Change the current running state.
		 */
		void changeState(string state);
    protected:
		//util functions
		/**
		 * Remove all geometry from the scene
		 */
        void clear();
        void newHiddenTile();
        /**
		 * Create a PathData object.
		 */
        PathData* log(osg::Vec3 pos);
        /**
		 * Test subject reached destination successfully.
		 */
        void reachedDestination();
        /**
		 * Test subject did not reach destination successfully.
		 */
        void timeOut();
        /**
		 * Write the most recent trial to a log file.
		 */
        void writeToLog();
        /**
		 * Create a new Path object for data recording.
		 */
        void createPath();
        /**
		 * The experiment is over.  Reset data.
		 */
        void endExperiment();
        /**
		 * Fade in effect.
		 */
        void fadeIn();
        /**
		 * Fade out effect.
		 */
        void fadeOut();
		/**
		 * Sync incoming data across nodes.
		 */
		int syncData();
		/**
		 * Get data from android controllers.
		 */
		void getData();
		/**
		 * After a trial has ended, change the state to the appropriate state.
		 */
		void trialEndStateChange();
		/**
		 * Used to disallow movement when movement is not allowed and to disallow the test subject to move through walls.
		 */
		void checkMovement();

		//Members
		static WaterMaze * _myPtr;

		//Menu Data
		cvr::SubMenu * _WaterMazeMenu, * _positionMenu, * _detailsMenu;
		cvr::MenuButton * _loadButton, * _clearButton, *_newTileButton;
		cvr::MenuCheckbox * _gridCB, * _wallColorCB, * _shapesCB, * _furnitureCB, *_lightingCB;
		std::vector<cvr::MenuButton *> _positionButtons;
		
		// Geometry
		osg::ref_ptr<osg::MatrixTransform> _geoRoot;	///< root of all non-GUI plugin geometry note: also referred to in this API as "scene space"
		osg::ref_ptr<osg::Switch> _gridSwitch, _wallColorSwitch, _wallWhiteSwitch,
			_shapeSwitch, _furnitureSwitch;
		std::map<osg::Vec3, osg::Switch *> _tileSwitches;

		//TODO: move this to be controlled by paradigm?
		float widthTile, heightTile, depth, wallHeight,
			gridWidth, _heightOffset;

		// Game logic
		int _currentParadigm;				///< index of the current Paradigm inside  \p _paradigms
		bool _runningTrial, _finished;		///< @todo refactor out _runningTrial and use state instead
		bool _resetTime;
		float _startTime;
		osg::Matrixd _prevMat;				///< used to prevent illegal movement during the experiment


		bool _debug, _loaded; // turns on debug messages to command line
		std::string _dataDir;				///< directory containing all necessary files to render the WaterMaze
		float _updateRate;					///< rate at which CalVR will push data points to the android controller. Default: ~3Hz
		float _androidTimer;				///< iteration timer.
		
		//data recording
		std::vector<Paradigm*> _paradigms;	///< internal data structure used to store all information regarding 
		Path* _currPath;
		
		float zeroZeroX, zeroZeroY;
		std::string _state;					///< string representation of current running state of the application (Note: these values correspond to state values in the android controller)
		std::string _currSubject;			///< unique identifier for a subject.  used for naming of the log file for the trials.
		
		//android controller
		WMController* _controller;	///< used for communication between application and android controller devices
		
		//Used for syncing across nodes.
		/**
		 * Communication between CalVR nodes using ComController 
		 * requires fixed size data, which eliminates the use of the 
		 * dynamic sized tuples returned from 
		 * \p WMController::getToggle
		 */
		struct CueToggle{
			char c[200];	///< c-string representing the 
			bool on;		///< State of the cue. 
		};
		
		//ending procedure data
		EndCue* _howToEnd;
		float _endingTimer;
		bool _finishing;
		
		//starting procedure data
		/**
		 * Used to centralize all the values needed to run the starting procedure.
		 */
		struct Start{
			float startTime;	///< point in time (since the launch of CalVR) that the starting procedure began
			float duration;		///< how long the starting procedure will last
		};
		
		StartCue* _howToStart;
		float _startingTimer;
		bool _starting;
		
		//Color filter used for fade effect
		osg::BlendColor* _filter;
		osg::BlendFunc* _filterFunc;
		float _filterA;
};

};

#endif

