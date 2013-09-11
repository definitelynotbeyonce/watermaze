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
#include <netdb.h>
#include <sys/socket.h>
#include <X11/Xlib.h>

#include "SCServer.hpp"
#include "Buffer.hpp"
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

        // USB to Serial communication
        /*HANDLE hSerial;
        FT_HANDLE ftHandle;
        FT_STATUS ftStatus;
        DWORD devIndex;
        DWORD bytesWritten;
        unsigned char buf[16];
        bool _sppConnected;
*/
		sc::SCServer * _aserver;
		sc::Buffer * _regTileBuf;
		sc::Buffer * _hiddenTileBuf;
		std::map<std::string, float> _regTileArgs;
		std::map<std::string, float> _hiddenTileArgs;
		int _curTile;

		//TODO: is this used?
		float randomFloat(float min, float max)
		{
			if (max < min) return 0;

			float random = ((float) rand()) / (float) RAND_MAX;
			float diff = max - min;
			float r = random * diff;
			return min + r;
		};

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
		bool _runningTrial, _resetTime;
		float _startTime;
		


		bool _debug, _loaded; // turns on debug messages to command line
		std::string _dataDir;
		std::string _dataFile;	//TODO: refactor out
		float _fileTick, _fileTimer;	//TODO: make _fileTick configurable
		
		//data recording
		std::vector<Paradigm*> _paradigms;	//TODO: once struct is refactored out, remove the 2
		Path* _currPath;
		
		float zeroZeroX, zeroZeroY;
		
		//android controller
		WMController* _controller;
		

};

};

#endif

