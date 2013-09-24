#ifndef WATERMAZE_PLUGIN_H
#define WATERMAZE_PLUGIN_H


#include <cvrKernel/ComController.h>
#include <cvrKernel/CVRPlugin.h>
#include <cvrKernel/FileHandler.h>
#include <cvrKernel/InteractionManager.h>
#include <cvrKernel/PluginHelper.h>
#include <cvrKernel/SceneManager.h>
#include <cvrKernel/SceneObject.h>

#include <cvrUtil/Intersection.h>
#include <cvrConfig/ConfigManager.h>

#include <cvrMenu/SubMenu.h>
#include <cvrMenu/MenuRangeValue.h>
#include <cvrMenu/MenuCheckbox.h>
#include <cvrMenu/MenuButton.h>
#include <cvrMenu/MenuText.h>
#include <cvrUtil/CVRSocket.h>
#include <cvrUtil/MultiListenSocket.h>

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

#include <string.h>
#include <vector>
#include <map>

#include <iostream>
#include <stdio.h>
//#include <netdb.h>
//#include <sys/socket.h>
//#include <X11/Xlib.h>

//#include "SCServer.hpp"
//#include "Buffer.hpp"
#include "Path.h"
#include "Paradigm.h"

#include "Controller/WMController.h"
#include "Controller/TrialSetup.h"



namespace WaterMaze
{
	class WMController;
class WaterMaze: public cvr::CVRPlugin, public cvr::MenuCallback
{
    public:
		//CVRPlugin overrides
        WaterMaze();
        virtual ~WaterMaze();
        static WaterMaze * instance();
        bool init();
        void menuCallback(cvr::MenuItem * item);
        void preFrame();
        bool processEvent(cvr::InteractionEvent * event);
		
		//state change functions
		void load();
		void changeParadigm(int direction);
		void changeTrial(int direction);
		void startStop();
		void playPause();
		void addTrial();
		//access methods
		string getState();
		TrialSetup* getTrialSetup();
		WMController* getController();
		
		void takeAction(int action);
    protected:
		//util functions
        void clear();
        void resetPosition();
        void newHiddenTile();
        void log(osg::Vec3* pos);
        void reachedDestination();
        void timeOut();
        void writeToLog();
        void createPath();
		void changeState(string state);
		int syncData();
		void getData();

		std::map<std::string, float> _regTileArgs;
		std::map<std::string, float> _hiddenTileArgs;
		int _curTile;

		static WaterMaze * _myPtr;

		cvr::SubMenu * _WaterMazeMenu, * _positionMenu, * _detailsMenu;
		cvr::MenuButton * _loadButton, * _clearButton, *_newTileButton;
		cvr::MenuCheckbox * _gridCB, * _wallColorCB, * _shapesCB, * _furnitureCB, *_lightingCB;
		std::vector<cvr::MenuButton *> _positionButtons;
		
		// Geometry
		osg::ref_ptr<osg::MatrixTransform> _geoRoot; // root of all non-GUI plugin geometry
		osg::ref_ptr<osg::Switch> _gridSwitch, _wallColorSwitch, _wallWhiteSwitch,
			_shapeSwitch, _furnitureSwitch;
		std::map<osg::Vec3, osg::Switch *> _tileSwitches;
		std::vector<osg::MatrixTransform *> _tilePositions;

		//TODO: move this to be controlled by paradigm?
		float widthTile, heightTile, depth, wallHeight,
			gridWidth, _heightOffset;

		// Game logic
		int _currentParadigm;
		bool _runningTrial, _finished;
		bool _resetTime;
		float _startTime;
		


		bool _debug, _loaded; // turns on debug messages to command line
		std::string _dataDir;
		std::string _dataFile;	//TODO: refactor out
		float _fileTick, _fileTimer;	//TODO: make _fileTick configurable
		
		//data recording
		std::vector<Paradigm*> _paradigms;	//TODO: once struct is refactored out, remove the 2
		Path* _currPath;
		
		float zeroZeroX, zeroZeroY;
		std::string state;
		
		//android controller
		WMController* _controller;
		

};

};

#endif

