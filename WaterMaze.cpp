#include "WaterMaze.h"
#include "Cues/WallCue.h"
#include <string>

using namespace cvr;
using namespace osg;
using namespace std;
//using namespace sc;

namespace WaterMaze
{

WaterMaze * WaterMaze::_myPtr = NULL;

CVRPLUGIN(WaterMaze)

WaterMaze::WaterMaze()
{
    _myPtr = this;
    _geoRoot = new osg::MatrixTransform();

   _curTile=0;

    float heightOffset = ConfigManager::getFloat("value", 
        "Plugin.WaterMaze.StartingHeight", 300.0);
	cout << "height: " << heightOffset << endl;
    osg::Matrixd mat;
    mat.makeTranslate(0, -3000, -heightOffset);
    _geoRoot->setMatrix(mat);
    PluginHelper::getObjectsRoot()->addChild(_geoRoot);
    
    _loaded = false;
    _currentParadigm = 0;
    _startTime = 0;
    _resetTime = true;
    _fileTick = .5;
    _runningTrial = false;

	
    std::cout << "Welcome to WaterMaze!\n" <<
			"l - load geometry\n" <<
			"n - next paradigm\n" <<
			"r - rerun trial\n" <<
			"b - previous paradigm\n" <<
			"s - start trial/abort trial and reset position\n" << 
			"p - play/pause\n" <<
			"f - next trial\n" <<	
			"a - override trial limit/add trial\n" <<
			"o - output results to log file\n" <<
			"i - info\n" <<
			"h - help menu" << std::endl;
	
	_controller = new WMController(this);
}

WaterMaze::~WaterMaze()
{
	if(_controller != NULL)
		delete _controller;
	if(_currPath != NULL)
		delete _currPath;
	
}

WaterMaze * WaterMaze::instance()
{
    return _myPtr;
}

bool WaterMaze::init()
{
    // Setup menus
    _WaterMazeMenu = new SubMenu("Water Maze");

    PluginHelper::addRootMenuItem(_WaterMazeMenu);

    _loadButton = new MenuButton("Load");
    _loadButton->setCallback(this);
    _WaterMazeMenu->addItem(_loadButton);

    _clearButton = new MenuButton("Clear");
    _clearButton->setCallback(this);
    _WaterMazeMenu->addItem(_clearButton);

    _newTileButton = new MenuButton("New Tile");
    _newTileButton->setCallback(this);
    _WaterMazeMenu->addItem(_newTileButton);

    _gridCB = new MenuCheckbox("Show Grid", false);
    _gridCB->setCallback(this);
    _WaterMazeMenu->addItem(_gridCB);

    // extra output messages
    _debug = (ConfigManager::getEntry("Plugin.WaterMaze.Debug") == "on");

    // Sync random number generator
    if(ComController::instance()->isMaster())
    {
        int seed = time(NULL);
		ComController::instance()->sendSlaves(&seed, sizeof(seed));
        srand(seed);
    } 
    else 
    {
        int seed = 0;
		ComController::instance()->readMaster(&seed, sizeof(seed));
        srand(seed);
    }
    
    // Set up models
    widthTile = ConfigManager::getFloat("value", "Plugin.WaterMaze.WidthTile", 2000.0);
    heightTile = ConfigManager::getFloat("value", "Plugin.WaterMaze.HeightTile", 2000.0);
    depth = ConfigManager::getFloat("value", "Plugin.WaterMaze.Depth", 10.0);
    wallHeight = ConfigManager::getFloat("value", "Plugin.WaterMaze.WallHeight", 2500.0);
    gridWidth = ConfigManager::getFloat("value", "Plugin.WaterMaze.GridWidth", 5.0);

    _dataDir = ConfigManager::getEntry("Plugin.WaterMaze.DataDir");

	//read in all the paradigms
	std::vector<std::string> paradigmNames;
    ConfigManager::getChildren("Plugin.WaterMaze.Paradigms", paradigmNames);
    for(int i = 0; i < paradigmNames.size(); ++i)
    {
		//cout << "get children : " << paradigmNames[i] << endl;
		string path = _dataDir + ConfigManager::getEntry("path", "Plugin.WaterMaze.Paradigms." + paradigmNames[i], "");
		string id = ConfigManager::getEntry("value", "Plugin.WaterMaze.Paradigms." + paradigmNames[i], "");
		Paradigm* p = new Paradigm(id, path);
		_paradigms.push_back(p);
	}
	
	//initialize current path
	_currPath = NULL;
	
	state = "default state";

    return true;
}

void WaterMaze::load()
{
	cout << "loading" << endl;
    _loaded = false;
	Paradigm* current = _paradigms[_currentParadigm];
	int numWidth = current->getWidth();
	int numHeight = current->getLength();
	
    // Set up models
	
    // Tiles
    osg::Box * box = new osg::Box(osg::Vec3(0,0,0), widthTile, heightTile, depth);
    for (int i = 0; i < numWidth; ++i)
    {
        for (int j = 0; j < numHeight; ++j)
        {
            osg::PositionAttitudeTransform * tilePat = new osg::PositionAttitudeTransform();
            tilePat->setPosition(osg::Vec3((widthTile*i) - (widthTile/2), 
                                           (heightTile*j) - (heightTile/2),
                                            0));
			
			//white
            osg::Switch * boxSwitch = new osg::Switch();
            osg::ShapeDrawable * sd = new osg::ShapeDrawable(box);
            sd->setColor(osg::Vec4(1, 1, 1, 1));
            osg::Geode * geode = new osg::Geode();
            geode->addDrawable(sd);
            boxSwitch->addChild(geode);

			//green
            sd = new osg::ShapeDrawable(box);
            sd->setColor(osg::Vec4(0, 1, 0, 1));
            geode = new osg::Geode();
            geode->addDrawable(sd);
            boxSwitch->addChild(geode);

			//red
            sd = new osg::ShapeDrawable(box);
            sd->setColor(osg::Vec4(1, 0, 0, 1));
            geode = new osg::Geode();
            geode->addDrawable(sd);
            boxSwitch->addChild(geode);

            tilePat->addChild(boxSwitch);
            _geoRoot->addChild(tilePat);

            osg::Vec3 center;
            center = tilePat->getPosition();
            _tileSwitches[center] = boxSwitch;
        }
    }
    
    // Grid
    _gridSwitch = new osg::Switch();
    for (int i = -1; i < numWidth; ++i)
    {
        box = new osg::Box(osg::Vec3(i * widthTile, heightTile * (numHeight-2) * .5, 0), 
            gridWidth, heightTile * numHeight, depth + 1);
        osg::ShapeDrawable * sd = new osg::ShapeDrawable(box);
        sd->setColor(osg::Vec4(0,0,0,1));
        osg::Geode * geode = new osg::Geode();
        geode->addDrawable(sd);
        _gridSwitch->addChild(geode);
    }

    for (int i = -1; i < numHeight; ++i)
    {
        box = new osg::Box(osg::Vec3(widthTile * (numWidth-2) * .5, i * heightTile, 0), 
            widthTile * numWidth, gridWidth, depth + 1);
        osg::ShapeDrawable * sd = new osg::ShapeDrawable(box);
        sd->setColor(osg::Vec4(0,0,0,1));
        osg::Geode * geode = new osg::Geode();
        geode->addDrawable(sd);
        _gridSwitch->addChild(geode);
    }
    _gridSwitch->setAllChildrenOff();
    _geoRoot->addChild(_gridSwitch); 


    // Walls
    osg::ShapeDrawable * sd;
    osg::Geode * geode;
    osg::Vec3 pos;
    
    _wallWhiteSwitch = new osg::Switch();
    _wallColorSwitch = new osg::Switch();
    _shapeSwitch = new osg::Switch();
    _furnitureSwitch = new osg::Switch();

    std::string wallTex = ConfigManager::getEntry("Plugin.WaterMaze.Textures.Wall");
    osg::Texture2D* tex = new osg::Texture2D();
    osg::ref_ptr<osg::Image> img;

    img = osgDB::readImageFile(_dataDir + wallTex);
    if (img)
    {
        tex->setImage(img);
        tex->setResizeNonPowerOfTwoHint(false);
        tex->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
        tex->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
    }


	
    TrialSetup* ts = getTrialSetup();
    current->renderCues(_geoRoot, ts);
    delete ts;


    // ceiling
    pos = osg::Vec3((numWidth-2) * widthTile * .5, 
                    (numHeight-2) * heightTile * .5, 
                    wallHeight);
    box = new osg::Box(pos, numWidth * widthTile, numHeight * heightTile, 5);
    sd = new osg::ShapeDrawable(box);
    geode = new osg::Geode();
    geode->addDrawable(sd);
    geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    geode->getOrCreateStateSet()->setTextureAttributeAndModes(0,tex,osg::StateAttribute::ON);
    _geoRoot->addChild(geode);
    
    // floor plane
    pos = osg::Vec3((numWidth-2) * widthTile * .5, 
                    (numHeight-2) * heightTile * .5, 
                    0);
    box = new osg::Box(pos, 200000, 200000, 5);
    sd = new osg::ShapeDrawable(box);
    sd->setColor(osg::Vec4(1, .8, .8, 1));
    geode = new osg::Geode();
    geode->addDrawable(sd);
    _geoRoot->addChild(geode);

    // sky box
    pos = osg::Vec3((numWidth-2) * widthTile * .5, 
                    (numHeight-2) * heightTile * .5, 
                    0);
    box = new osg::Box(pos, 200000, 200000, 200000);
    sd = new osg::ShapeDrawable(box);
    sd->setColor(osg::Vec4(.8, .8, 1, 1));
    geode = new osg::Geode();
    geode->addDrawable(sd);
    _geoRoot->addChild(geode);	

	//starting location
	float startingX = current->getStartingPos()/numWidth;
	startingX = startingX * widthTile - (widthTile/2);
	float startingY = current->getStartingPos()%numWidth;
	startingY = startingY * heightTile - (heightTile/2);
	
	/*
	cout << current->getStartingPos()/numWidth << " " << current->getStartingPos()%numWidth << endl;
	cout << "starting x: " << startingX << " y: " << startingY << endl;*/
	
	osg::Matrixd mat;
	mat.makeTranslate(osg::Vec3(-startingX, -startingY,0));
	PluginHelper::setObjectMatrix(mat);

	_loaded = true;
	changeState("geometry loaded");

}



void WaterMaze::preFrame()
{
	//controller handling
	getData();
	int data = syncData();
	
	takeAction(data);
	
    if (!_runningTrial)
        return;
    
    checkMovement();
    
    if(!_loaded)
		return;
    //handle timeout
    if (_paradigms[_currentParadigm]->getTimeRemaining(PluginHelper::getProgramDuration() - _startTime) <= 0)
		timeOut();
    
	
    osg::Vec3 pos = osg::Vec3(0,0,0) * cvr::PluginHelper::getHeadMat() * 
        PluginHelper::getWorldToObjectTransform() * _geoRoot->getInverseMatrix();

    osg::Vec3 bottomLeft, topRight;
    bottomLeft = osg::Vec3(0,0,0) * _geoRoot->getMatrix();
    topRight = osg::Vec3(widthTile * _paradigms[_currentParadigm]->getWidth(), 
                         heightTile * _paradigms[_currentParadigm]->getLength(), 0) * 
                         _geoRoot->getMatrix();

	//used as bounding geometry for grid cells later
    float xmin, xmax, ymin, ymax;
    
    bool logged = false;
    if (PluginHelper::getProgramDuration() - _fileTimer > _fileTick)
    {
		//record data 
		logged = true;
		log(&pos);
		_fileTimer = PluginHelper::getProgramDuration();
	}
    
    int i = 0;
    std::map<osg::Vec3, osg::Switch *>::iterator it;
    for (it = _tileSwitches.begin(); it != _tileSwitches.end(); ++it)
    {
        osg::Vec3 center = it->first;
        xmin = center[0] - widthTile/2;
        xmax = center[0] + widthTile/2;
        ymin = center[1] - heightTile/2;
        ymax = center[1] + heightTile/2;

        
        // Occupied tile
        if (pos[0] > xmin && pos[0] < xmax &&
            pos[1] > ymin && pos[1] < ymax)
        {
            //std::cout << "Standing in tile " << i << std::endl;  
            if (it->second)
            {
                if (i == _paradigms[_currentParadigm]->getFinishPos())
                {
                    it->second->setSingleChildOn(1);
                    reachedDestination();
                    //unless we just logged, log.
                    if(!logged)
                    {
						log(&pos);
					}
                    
                }
                else
                {
					if(_paradigms[_currentParadigm]->isColoredGrid())
						it->second->setSingleChildOn(2);
				}
            }
        }
        // Unoccupied tile
        else
        {
            if (it->second)
                it->second->setSingleChildOn(0);
        }
        ++i;
    }
}

void WaterMaze::menuCallback(MenuItem * item)
{
    if(item == _loadButton)
    {
        if (!_loaded)
            load();
    }

    else if (item == _clearButton)
    {
        clear();
    }

    else if (item == _newTileButton)
    {
        newHiddenTile();
    }


    else if (item == _gridCB)
    {
        if (_gridCB->getValue())
        {
            _gridSwitch->setAllChildrenOn();
        }
        else
        {
            _gridSwitch->setAllChildrenOff();
        }
    }
}

bool WaterMaze::processEvent(InteractionEvent * event)
{
    KeyboardInteractionEvent * kie = event->asKeyboardEvent();
    if (kie)
    {
		if (kie->getInteraction() == KEY_UP)
		{
			switch(kie->getKey())
			{
				case 'n':
					//next paradigm
					changeParadigm(1);
					break;
				case 'b':
					changeParadigm(-1);
					break;
				case 'p':
					// pause/play
					playPause();
					break;
				case 'r':	//TODO: deprecated control
					//rerun trial (throw out previous data)
					if (_runningTrial)
						return true;
					//delete results
					if (_currPath)
					{
						delete _currPath;
						load();
					}
					break;
				case 'f':
					//next trial
					changeTrial(1);
					break;
				case 'a':
					//override trial limit add trial
					addTrial();
					break;
				case 's':
					// start/abort trail and reset position
					startStop();
					break;
				case 'l':
					//load geometry
					cout << "Loading Geometry" << endl;
					load();
					break;
				case 'o':
					//output to log file
					writeToLog();
					break;
				case 'i':
					//info
					cout << "Paradigm: " << _currentParadigm << endl;
					cout << "Trial: " << _paradigms[_currentParadigm]->getTrialNumber() << endl;
					cout << "Running: " << _runningTrial << endl << endl;
					break;
				case 'h':
					//help menu
					cout << "n - next paradigm\n" <<
								 "b - previous paradigm\n" <<
								 "p - play/pause\n" <<
								 "r - rerun trial\n" <<
								 "f - next trial\n" <<	
								 "a - override trial limit/add trial\n" <<
								 "s - start trial/abort trial and reset position\n" << 
								 "l - load geometry\n" <<
								 "o - output results to log file\n" <<
								 "i - info\n" <<
								 "h - help menu" << endl;
					break;
			}
		}
    }
    
    TrackedButtonInteractionEvent * tie = event->asTrackedButtonEvent();
    if (tie)
    {
        if(tie->getHand() == 0 && tie->getButton() == 0)
        {
            if (tie->getInteraction() == BUTTON_DOWN && !_runningTrial)
            {
                return true;
            }
            else if (tie->getInteraction() == BUTTON_DRAG && !_runningTrial)
            {
                return true;
            }
            else if (tie->getInteraction() == BUTTON_UP)
            {
                return false;
            }
            return false;
        }
    }
    return false;
}

void WaterMaze::clear()
{
    _loaded = false;
    PluginHelper::getObjectsRoot()->removeChild(_geoRoot);

    _tileSwitches.clear();
    _tilePositions.clear();

    _geoRoot = new osg::MatrixTransform();

    float heightOffset = ConfigManager::getFloat("value", 
        "Plugin.WaterMaze.StartingHeight", 300.0);
    osg::Matrixd mat;
    mat.makeTranslate(0, -3000, -heightOffset);
    _geoRoot->setMatrix(mat);
    PluginHelper::getObjectsRoot()->addChild(_geoRoot);
}

//TODO: use this instead of clear() load() for moving between trials. just kidding
void WaterMaze::resetPosition()
{
	
}

void WaterMaze::log(osg::Vec3* pos)
{
	//get all the data
	float elapsedTime = PluginHelper::getProgramDuration() - _startTime;
	PathData* pd = new PathData(pos, elapsedTime);	//TODO pass in the other vectors for eye data.
	
	//insert
	_currPath->addStep(elapsedTime, pd);	
	
	//inform android
	_controller->bCastOutboundPacket(pd);
}

void WaterMaze::changeParadigm(int direction)
{
	if(_runningTrial)
		return;
		
	if((_currentParadigm + direction) < 0 
		|| (_currentParadigm + direction) > _paradigms.size() - 1)
	{
		if(direction < 0)
			std::cout << "this is the first paradigm" << std::endl;
		else
			std::cout << "No more paradigms" << std::endl;
		return;
	}
	
	//all went well we can change the paradigm
	_currentParadigm += direction;
	if(direction < 0)
		std::cout << "loading previous paradigm" << std::endl;
	else
		std::cout << "loading next paradigm" << std::endl;
	
	
	
	clear();
	if(_paradigms[_currentParadigm]->isAutoLoad())
		load();
	else
		changeState("experiment start");
}

void WaterMaze::changeTrial(int direction)
{
	if(_runningTrial)
		return;
	
	//figure out if we can
	if(_paradigms[_currentParadigm]->isContinuous())
	{
		//we can
		//store path
		//TODO: should null paths be allowed?
		_paradigms[_currentParadigm]->addPath(_currPath);
		_currPath = NULL;
		
		_paradigms[_currentParadigm]->setStartingPos();
		_paradigms[_currentParadigm]->setFinishPos();
		clear();
		if(_paradigms[_currentParadigm]->isAutoLoad())
			load();
		else
			changeState("experiment start");
		
	}
	else if(_paradigms[_currentParadigm]->trialsRemaining() > 0)
	{
		//store path
		//TODO: should null paths be allowed?
		if(_currPath == NULL)
			cout << "no path recorded adding null" << endl;
			
		_paradigms[_currentParadigm]->addPath(_currPath);
		_currPath = NULL;
		
		_paradigms[_currentParadigm]->setStartingPos();
		_paradigms[_currentParadigm]->setFinishPos();
		clear();
		if(_paradigms[_currentParadigm]->isAutoLoad())
			load();
		else
			changeState("experiment start");
			
	}
	else
	{
		cout << "Trial limit reached.  press \"a\" to add an additional trial." << endl;
	}	
}

//TODO
void WaterMaze::reachedDestination()
{
	//disable movement.
	cout << "Congratulations you have reached the destination!\n"
		 << "Press \"f\" to move to the next trial.\n" 
		 << "Or Press \"r\" to rerun trial" << endl;
	
	trialEndStateChange();
	
}

//TODO
void WaterMaze::timeOut()
{
	//stop running trial
	cout << "Congratulations you have reached the destination!\n"
		 << "Press \"f\" to move to the next trial." << endl;
		 
	trialEndStateChange();
}

void WaterMaze::trialEndStateChange()
{
	//get ending
	_howToEnd = _paradigms[_currentParadigm]->getEnding();
	_endingTimer = PluginHelper::getProgramDuration();
	
	if(_howToEnd == NULL)
		_runningTrial = false;
	if(_paradigms[_currentParadigm]->isContinuous())
		changeState("trial end continuous");
	else if(_paradigms[_currentParadigm]->trialsRemaining() > 0)
		changeState("trial end");
	else
	{
		if(_currentParadigm == _paradigms.size() - 1) 
			changeState("experiment end");
		else
			changeState("paradigm end");
	}
			
}

void WaterMaze::writeToLog()
{
	if(_currentParadigm == _paradigms.size() - 1
		&& _paradigms[_currentParadigm]->trialsRemaining() <= 0)
	{
		//get day/time
		time_t now = time(0);
		tm* curr = gmtime(&now);
		
		//file name
		char fileName[100];
		sprintf(fileName, "%sLogs/%s-%i-%i-%i-%i:%i.txt", 
			_dataDir.c_str(),
			"test42", 
			(curr->tm_mon + 1),
			(curr->tm_mday + 1),
			(curr->tm_year + 1900),
			((23 + curr->tm_hour)%24 + 1),
			(curr->tm_min + 1)); 
		
		ofstream outFile;
        outFile.open(fileName, ios::app);

        if (outFile) 
        {
			outFile << "test42" << endl;	//TODO: subject id is not "test42"
			outFile << _currentParadigm + 1 << "-";
			for (int i = 0; i < _paradigms.size(); ++i)
			{
				outFile << _paradigms[i]->getID();
				if(i != _paradigms.size() - 1)
					outFile << ",";
			}
			outFile << endl;
			
			for (int i = 0; i < _paradigms.size(); ++i)
			{
				_paradigms[i]->writeToLog(outFile);
			}
			outFile << endl;
			outFile << flush;
			outFile.close();
        }
        else
        {
            cout << "WaterMaze: Unable to open log file " << fileName << endl;
        }
	}
	else
	{
		cout << "Could not output log.  There are still tests to be done." << endl;
	}
}

void WaterMaze::startStop()
{
	if (_runningTrial)
	{
		timeOut();
		/*
		std::cout << "Aborting trial" << std::endl; 
		_runningTrial = false;
		_resetTime = true;
		
		//erase path data
		if (_currPath)
		{
			delete _currPath;
		}
		_currPath = NULL;
		
		//reset environment
		clear();
		load();
		* */
	}
	else
	{
		cout << "Starting trial" << endl; 
		_startTime = PluginHelper::getProgramDuration();
		_fileTimer = PluginHelper::getProgramDuration();
		createPath();
	}
}

string WaterMaze::getState()
{
	return state;
}

void WaterMaze::changeState(string state)
{
	//internal
	this->state = state;
	//inform devices
	StateUpdate* update = new StateUpdate(state);
	_controller->bCastOutboundPacket(update);
	delete update;
}

TrialSetup* WaterMaze::getTrialSetup()
{
	TrialSetup* ts = new TrialSetup();
	ts->trialNo = _paradigms[_currentParadigm]->getTrialNumber();
	ts->gridSize = widthTile;
	ts->length = _paradigms[_currentParadigm]->getLength();
	ts->width = _paradigms[_currentParadigm]->getWidth();
	ts->zero_zeroX = -widthTile;
	ts->zero_zeroY = -widthTile;
	ts->startingPos = _paradigms[_currentParadigm]->getStartingPos();
	ts->finishPos = _paradigms[_currentParadigm]->getFinishPos();
	ts->wallHeight = wallHeight;
	
	return ts;
}

void WaterMaze::playPause()
{
	if (_runningTrial)
	{
		std::cout << "Paused." << std::endl; 
		_runningTrial = false;
	}
	else
	{
		cout << "Play." << endl; 
		_runningTrial = true;
		
		changeState("running trial");
		
		if (_resetTime)
		{
			_startTime = PluginHelper::getProgramDuration();
			_resetTime = false;

			_fileTimer = PluginHelper::getProgramDuration();
			
			//create a new path.
			createPath();
		}
	}
}

void WaterMaze::addTrial()
{
	if(_runningTrial)
		return;
	if(_paradigms[_currentParadigm]->trialsRemaining() > 0)
		return;
	_paradigms[_currentParadigm]->addTrial();
	changeState("trial end");
}

void WaterMaze::createPath()
{
	//save starting position
	_prevMat = PluginHelper::getObjectMatrix();
	
	if (_currPath)
	{
		delete _currPath;
	}
	_currPath = new Path();
	
	//inform android
	changeState("running trial");
	TrialSetup* ts = getTrialSetup();
	_controller->bCastOutboundPacket(ts);
	delete ts;
	
	_runningTrial = true;
}

WMController* WaterMaze::getController()
{
	return _controller;
}

int WaterMaze::syncData()
{
	// Sync
    if(ComController::instance()->isMaster())
    {
		//chack controller for data.
		bool hasData = _controller->hasData();
		ComController::instance()->sendSlaves(&hasData, sizeof(hasData));
		
		if(hasData)
		{
			//consume from controller
			int data = _controller->getDataAsInt();
			//broadcast
			ComController::instance()->sendSlaves(&data, sizeof(data));
			return data;
		}
    } 
    else 
    {
        bool hasData;
		ComController::instance()->readMaster(&hasData, sizeof(hasData));
		
		if(hasData)
		{
			int data = 0;
			ComController::instance()->readMaster(&data, sizeof(data));
			return data;
		}
    }
    
    return 0;
}

void WaterMaze::getData()
{
	if(ComController::instance()->isMaster())
    {
		//chack controller for data.
		_controller->getConnections();
		_controller->getData();
    } 
}

CueList* WaterMaze::getCueList()
{
	//make a CueList
	CueList* list = new CueList(_paradigms[_currentParadigm]->getCues());
	return list;
}

void WaterMaze::syncToggle()
{
	// Sync
    if(ComController::instance()->isMaster())
    {
		//chack controller for data.
		bool hasToggles = _controller->hasToggles();
		ComController::instance()->sendSlaves(&hasToggles, sizeof(hasToggles));
		
		if(hasToggles)
		{
			//consume from controller
			tuple<string, bool> data = _controller->getToggle();
			struct CueToggle cueToggle;
			size_t t = min((size_t)200, get<0>(data).length() + 1);
			memcpy(cueToggle.c, get<0>(data).c_str(), t);
			cueToggle.on = get<1>(data);
			
			//broadcast
			ComController::instance()->sendSlaves(&cueToggle, sizeof(cueToggle));
			_paradigms[_currentParadigm]->toggle(data);
			
			CueList* cl = getCueList();
			_controller->bCastOutboundPacket(cl);
			delete cl;
		}
    } 
    else 
    {
        bool hasToggles;
		ComController::instance()->readMaster(&hasToggles, sizeof(hasToggles));
		
		if(hasToggles)
		{
			tuple<string, bool> data;
			struct CueToggle cueToggle;
			ComController::instance()->readMaster(&cueToggle, sizeof(cueToggle));
			data = make_tuple<string, bool>(string(cueToggle.c), cueToggle.on);
			_paradigms[_currentParadigm]->toggle(data);
		}
    }
}

void WaterMaze::syncNewSubject()
{
	if(ComController::instance()->isMaster())
    {
		//chack controller for data.
		bool hasToggles = _controller->hasNewSubjects();
		ComController::instance()->sendSlaves(&hasToggles, sizeof(hasToggles));
		
		if(hasToggles)
		{
			string s = _controller->getNewSubject();
			int size = s.length() + 1;
			char istia[size];
			ComController::instance()->sendSlaves(&size, sizeof(size));
			ComController::instance()->sendSlaves(&istia, sizeof(istia));
			_currSubject = string(s);
			changeState("experiment start");
		}
    } 
    else 
    {
        bool hasToggles;
		ComController::instance()->readMaster(&hasToggles, sizeof(hasToggles));
		
		if(hasToggles)
		{
			tuple<string, bool> data;
			int size = 0;
			ComController::instance()->readMaster(&size, sizeof(size));
			char istia[size];
			ComController::instance()->readMaster(&istia, sizeof(istia));
			_currSubject = string(istia);
			changeState("experiment start");
		}
    }
}

void WaterMaze::checkMovement()
{
	//movement disabled. 
	if(_howToEnd != NULL)
	{
		osg::Matrixd mat = PluginHelper::getObjectMatrix();
		if(PluginHelper::getProgramDuration() - _endingTimer >= _howToEnd->getDuration())
		{
			_runningTrial = false;
			_howToEnd = NULL;
		}
		else
		{
			if(_loaded && !_howToEnd->isGeometryVisible())
				clear();
			else
			{
				if(!_howToEnd->isMotionAllowed())
				{
					PluginHelper::setObjectMatrix(_prevMat);
				}
				else
				{
					float boundSize = 100;
					//check collisions.
					//get position matrix
					osg::Vec3 pos = osg::Vec3(0,0,0) * cvr::PluginHelper::getHeadMat() * 
						PluginHelper::getWorldToObjectTransform() * _geoRoot->getInverseMatrix();
					
					osg::Vec3 origin = osg::Vec3(-widthTile, -heightTile, 0);
					
					Vec3 c = pos - origin;	
					
					
					//check bounds of c
					cout << "x: " << c.x() << " y: " << c.y() << endl;
					if((c.x() > boundSize)
						&& (c.x() < _paradigms[_currentParadigm]->getWidth() * widthTile - boundSize)
						&& (c.y() > boundSize)
						&& (c.y() <heightTile * _paradigms[_currentParadigm]->getLength()))	//this means it's inside
					{
						_prevMat = PluginHelper::getObjectMatrix();
					}
					else
					{
						PluginHelper::setObjectMatrix(_prevMat);
					}
				}
			}
		}
		
	}
	else
	{
		float boundSize = 100;
		//check collisions.
		//get position matrix
		osg::Vec3 pos = osg::Vec3(0,0,0) * cvr::PluginHelper::getHeadMat() * 
			PluginHelper::getWorldToObjectTransform() * _geoRoot->getInverseMatrix();
		
		osg::Vec3 origin = osg::Vec3(-widthTile, -heightTile, 0);
		
		Vec3 c = pos - origin;	
		
		
		//check bounds of c
		cout << "x: " << c.x() << " y: " << c.y() << endl;
		if((c.x() > boundSize)
			&& (c.x() < _paradigms[_currentParadigm]->getWidth() * widthTile - boundSize)
			&& (c.y() > boundSize)
			&& (c.y() <heightTile * _paradigms[_currentParadigm]->getLength()))	//this means it's inside
		{
			_prevMat = PluginHelper::getObjectMatrix();
		}
		else
		{
			PluginHelper::setObjectMatrix(_prevMat);
		}
	}
}

};
