#include "WaterMaze.h"
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
    
    //filter material
    _filter = new BlendColor(Vec4(0.0, 0.0, 0.0, 0.0));
    _filterFunc = new BlendFunc();
    _filterA = 0.0;
    _filterFunc->setSource(osg::BlendFunc::CONSTANT_ALPHA);
    _filterFunc->setDestination(osg::BlendFunc::ONE_MINUS_CONSTANT_ALPHA);
	_geoRoot->getOrCreateStateSet()->setAttributeAndModes(_filterFunc, osg::StateAttribute::ON);
	_geoRoot->getOrCreateStateSet()->setAttributeAndModes(_filter, osg::StateAttribute::ON);

	//initialize scene space
    float heightOffset = ConfigManager::getFloat("value", 
        "Plugin.WaterMaze.StartingHeight", 1663);	//CalVR for some reason has a default height of 1663
	cout << "height: " << heightOffset << endl;
    osg::Matrixd mat;
    mat.makeTranslate(0, 0, -heightOffset);
    _geoRoot->setMatrix(mat);
    PluginHelper::getObjectsRoot()->addChild(_geoRoot);
    
    //initialize flags
    _loaded = false;
    _currentParadigm = 0;
    _startTime = 0;
    _resetTime = true;
    _updateRate= ConfigManager::getFloat("Plugin.WaterMaze.AndroidController.UpdateRate", 0.333);
    _runningTrial = false;

	//keyboard control prompt
    std::cout << "Welcome to WaterMaze!\n" <<
			"l - load geometry\n" <<
			"n - proceed to next paradigm\n" <<
			"s - start/stop trial\n" << 
			"f - proceed to next trial\n" <<	
			"a - override trial limit/add trial\n" <<
			"i - info\n" <<
			"h - help menu" << std::endl;
}

WaterMaze::~WaterMaze()
{
	changeState("disconnected");
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
    
    // initialize grid constants
    widthTile = ConfigManager::getFloat("value", "Plugin.WaterMaze.WidthTile", 2000.0);
    heightTile = ConfigManager::getFloat("value", "Plugin.WaterMaze.HeightTile", 2000.0);
    depth = ConfigManager::getFloat("value", "Plugin.WaterMaze.Depth", 10.0);
    wallHeight = ConfigManager::getFloat("value", "Plugin.WaterMaze.WallHeight", 2500.0);
    gridWidth = ConfigManager::getFloat("value", "Plugin.WaterMaze.GridWidth", 5.0);

	//Location of data files.
    _dataDir = ConfigManager::getEntry("Plugin.WaterMaze.DataDir");

	//read in all the paradigms
	std::vector<std::string> paradigmNames;
    ConfigManager::getChildren("Plugin.WaterMaze.Paradigms", paradigmNames);
    for(int i = 0; i < paradigmNames.size(); ++i)
    {
		string path = _dataDir + ConfigManager::getEntry("path", "Plugin.WaterMaze.Paradigms." + paradigmNames[i], "");
		string id = ConfigManager::getEntry("value", "Plugin.WaterMaze.Paradigms." + paradigmNames[i], "");
		Paradigm* p = new Paradigm(id, path);
		_paradigms.push_back(p);
	}
	
	//initialize current path
	_currPath = NULL;
	
	//initialize android controller
	if(ComController::instance()->isMaster())
		_controller = new WMController(this);
	else
		_controller = NULL;
	
	changeState("disconnected");
	
	//default background color
	PluginHelper::setClearColor(Vec4(0.0, 0.0, 0.0, 1.0));	//Black
	
	//master volume
	_masterVolume = ConfigManager::getFloat("Plugins.WaterMaze.MasterVolume", 1.0);
	cout << "Master Volume: " << _masterVolume << endl;
	
	//DEBUG
	maxVel = 0;
	
	//Movement limitation
	float maxAllowed = ConfigManager::getFloat("Plugin.WaterMaze.MaxVelocity", 1.0);
	float maxAble = ConfigManager::getFloat("Plugin.WaterMaze.DefaultMax", maxAllowed);
	Navigation::instance()->setScale(maxAllowed / maxAble);
	
	cout << endl << "WaterMaze Init Done." << endl << endl;
    return true;
}

void WaterMaze::load()
{
	//get starting procedure (how the geometry should be introduced)
	
	cout << "loading geometry for paradigm " << _paradigms[_currentParadigm]->getID() << endl;
	
	//load maze dimensions
    _loaded = false;
	Paradigm* current = _paradigms[_currentParadigm];
	int numWidth = current->getWidth();
	int numHeight = current->getLength();
	
	//Set the start and end points
	_paradigms[_currentParadigm]->setStartingPos();
	_paradigms[_currentParadigm]->setFinishPos();
		
    // Set up models
	
		// Tiles
	//tile texture
	std::string textureFile = ConfigManager::getEntry("Plugin.WaterMaze.Textures.Floor");
    osg::Texture2D* floorTex = new osg::Texture2D();
    osg::ref_ptr<osg::Image> floorImg;

    floorImg = osgDB::readImageFile(ConfigManager::getEntry("Plugin.WaterMaze.DataDir") + textureFile);
    if (floorImg)
    {
        floorTex->setImage(floorImg);
        floorTex->setResizeNonPowerOfTwoHint(false);
        floorTex->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
        floorTex->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
    }
    
	//geometry used for each tile		
    osg::Box * box = new osg::Box(osg::Vec3(0,0,0), widthTile, heightTile, depth);
    
    //insert each tile
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
			geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
			geode->getOrCreateStateSet()->setTextureAttributeAndModes(0,floorTex,osg::StateAttribute::ON);
			
            boxSwitch->addChild(geode);

			//green
			if(_paradigms[_currentParadigm]->isTileCircle())
			{
				//green destination tile is actually a circle
				geode = new osg::Geode();
				osg::Cylinder * circle = new osg::Cylinder(osg::Vec3(0,0,0), (sqrt(widthTile*widthTile + heightTile*heightTile) /2), depth + 2);
				sd = new osg::ShapeDrawable(circle);
				sd->setColor(osg::Vec4(0, 1, 0, 1));
				geode->addDrawable(sd);
				
				boxSwitch->addChild(geode);
			}
			else
			{
				//green destination tile is a square
				sd = new osg::ShapeDrawable(box);
				sd->setColor(osg::Vec4(0, 1, 0, 1));
				geode = new osg::Geode();
				geode->addDrawable(sd);
				boxSwitch->addChild(geode);
			}

			//red
            sd = new osg::ShapeDrawable(box);
            sd->setColor(osg::Vec4(1, 0, 0, 1));
            geode = new osg::Geode();
            geode->addDrawable(sd);
            boxSwitch->addChild(geode);

			//turn all of the children off
			boxSwitch->setAllChildrenOff();
			boxSwitch->setSingleChildOn(0);
			
			//add the switch to the PAT
            tilePat->addChild(boxSwitch);
            _geoRoot->addChild(tilePat);

			//add this to the array of switches
            osg::Vec3 center;
            center = tilePat->getPosition();
            _tileSwitches[center] = boxSwitch;
        }
    }
    
    // Grid Lines
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
    //_geoRoot->addChild(geode);

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

	//Move head to starting location in scene space
	float startingX = current->getStartingPos()/numHeight;
	startingX = startingX * widthTile - (widthTile/2);
	float startingY = current->getStartingPos()%numHeight;
	startingY = startingY * heightTile - (heightTile/2);
	
	osg::Matrixd mat;
	Vec3 p1 = osg::Vec3(-startingX, -startingY,0);
	mat.makeTranslate(0,0,0);
	PluginHelper::setObjectMatrix(mat);
	print(p1, "target position");
	
	//Ensure that the head is at (0, 0)
	osg::Vec3 p2 = osg::Vec3(0,0,0) * cvr::PluginHelper::getHeadMat()
		* PluginHelper::getWorldToObjectTransform() * _geoRoot->getInverseMatrix();
    p2[2] = 0;
    mat.makeTranslate(p1 + p2);
    PluginHelper::setObjectMatrix(mat);    
    p2 = osg::Vec3(0,0,0) * cvr::PluginHelper::getHeadMat() * PluginHelper::getWorldToObjectTransform() * _geoRoot->getInverseMatrix();
    print (p2, "adjusted head position");
    _prevMat = mat;

	//color filter
	
	_loaded = true;
	changeState("geometry loaded");

}



void WaterMaze::preFrame()
{
	//controller handling
	getData();
	int data = syncData();
	takeAction(data);
	
	if(_finishing)
		handleEnding();
	else if(_starting)
		handleStart();
	
	if(!_loaded || !_runningTrial)
	{
		//disallow movement
		PluginHelper::setObjectMatrix(_prevMat);

		if(!_loaded)
			return;
	}
	
	if(_starting)
		handleStart();
		
    if (!_runningTrial)
        return; 
    
    //checkMovement();
    
    
    //handle timeout
    if (_paradigms[_currentParadigm]->getTimeRemaining(PluginHelper::getProgramDuration() - _startTime) <= 0)
		timeOut();
		
	//play tiem based sounds
	_paradigms[_currentParadigm]->playSoundCues(PluginHelper::getProgramDuration() - _startTime);
	
	//calculate position
    osg::Vec3 pos = osg::Vec3(0,0,0) * cvr::PluginHelper::getHeadMat() * 
        PluginHelper::getWorldToObjectTransform() * _geoRoot->getInverseMatrix();
    
    //play movment based sounds    
    _paradigms[_currentParadigm]->playSoundCues(pos);

    osg::Vec3 bottomLeft, topRight;
    bottomLeft = osg::Vec3(0,0,0) * _geoRoot->getMatrix();
    topRight = osg::Vec3(widthTile * _paradigms[_currentParadigm]->getWidth(), 
                         heightTile * _paradigms[_currentParadigm]->getLength(), 0) * 
                         _geoRoot->getMatrix();

	//used as bounding geometry for grid cells later
    float xmin, xmax, ymin, ymax;
    
    //Data Recording
    PathData* pd = log(pos);	//always log to internal data structure
	
	bool androided = false;	
    if (PluginHelper::getProgramDuration() - _androidTimer > _updateRate)
    {
		_androidTimer = PluginHelper::getProgramDuration();
		
		//push to android
		if(ComController::instance()->isMaster())
			_controller->bCastOutboundPacket(pd);
		
		androided = true;
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
            if (it->second)
            {
                if (i == _paradigms[_currentParadigm]->getFinishPos())
                {
                    it->second->setSingleChildOn(1);
                    
                    //inform android if we have not already done so
                    if(ComController::instance()->isMaster() && !androided)
						_controller->bCastOutboundPacket(pd);
						
                    reachedDestination();
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
					load();
					break;
				case 'i':
					/* info */
					cout << endl << endl;
					//running state of application
					cout << "Current Running State: " << _state << endl;
					//Paradigm ID
					cout << "Current Paradigm: " << _paradigms[_currentParadigm]->getID() << endl;
					//trials ran
					cout << "Trial: " << _paradigms[_currentParadigm]->getTrialNumber() << endl;
					//android controller data
					 if(ComController::instance()->isMaster())
						cout << "# of Connected Devices: " << _controller->getNumControllers() << endl;
					//volume
					cout << "Master Volume: " << _masterVolume << endl;
					break;
				case 'h':
					//help menu
					 std::cout << "Welcome to WaterMaze!\n" <<
								  "l - load geometry\n" <<
								  "n - proceed to next paradigm\n" <<
								  "s - start/stop trial\n" << 
								  "f - proceed to next trial\n" <<	
								  "a - override trial limit/add trial\n" <<
								  "i - info\n" <<
								  "h - help menu" << std::endl;
					break;
				case osgGA::GUIEventAdapter::KeySymbol::KEY_Up:
					//raise master volume
					cout << "Raising Volume" << endl;
					_masterVolume = min(1.0, _masterVolume + 0.1);
					for(int i = 0; i < _paradigms.size(); ++i)
					{
						_paradigms[i]->setVolume(_masterVolume);
					}
					break;
				case osgGA::GUIEventAdapter::KeySymbol::KEY_Down:
					//lower master volume
					cout << "Lowering Volume" << endl;
					_masterVolume = max(0.0, _masterVolume - 0.1);
					for(int i = 0; i < _paradigms.size(); ++i)
					{
						_paradigms[i]->setVolume(_masterVolume);
					}
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

    _geoRoot = new osg::MatrixTransform();
    
    //redo filter material
	_filter = new BlendColor(Vec4(0.0, 0.0, 0.0, 0.0));	//initial alpha of 0.0
	_filterFunc = new BlendFunc();
	_filterFunc->setSource(osg::BlendFunc::CONSTANT_ALPHA);
	_filterFunc->setDestination(osg::BlendFunc::ONE_MINUS_CONSTANT_ALPHA);
	_geoRoot->getOrCreateStateSet()->setAttributeAndModes(_filterFunc, osg::StateAttribute::ON);
	_geoRoot->getOrCreateStateSet()->setAttributeAndModes(_filter, osg::StateAttribute::ON);
	
    float heightOffset = ConfigManager::getFloat("value", 
        "Plugin.WaterMaze.StartingHeight", 300.0);
    osg::Matrixd mat;
    mat.makeTranslate(0, -3000, -heightOffset);
    _geoRoot->setMatrix(mat);
    PluginHelper::getObjectsRoot()->addChild(_geoRoot);
}

PathData* WaterMaze::log(osg::Vec3 pos)
{
	//get all the additional data
	
	
	//right
	Vec3 right = Vec3(1,0,0) * cvr::PluginHelper::getHeadMat() * 
        PluginHelper::getWorldToObjectTransform() * _geoRoot->getInverseMatrix();
	//front
	Vec3 front = Vec3(0,1,0) * cvr::PluginHelper::getHeadMat() * 
        PluginHelper::getWorldToObjectTransform() * _geoRoot->getInverseMatrix();
	//up
	Vec3 up = Vec3(0,0,1) * cvr::PluginHelper::getHeadMat() * 
        PluginHelper::getWorldToObjectTransform() * _geoRoot->getInverseMatrix();
        
    //time
    float elapsedTime = PluginHelper::getProgramDuration() - _startTime;
    
	PathData* pd = new PathData(pos, right, front, up, elapsedTime);	
	
	//insert
	_currPath->addStep(elapsedTime, pd);	
	
	return pd;
}

void WaterMaze::changeParadigm(int direction)
{
	if(_runningTrial)
		return;
		
	if((_currentParadigm + direction) < 0  
		|| (_currentParadigm + direction) > _paradigms.size() - 1)
	{
		//there are no more paradigms
		return;
	}
	
	//all went well we can change the paradigm
	_currentParadigm += direction;
	clear();
	
	//inform android of the change in paradigms
	if(ComController::instance()->isMaster())
	{
		GeneralComm* gc = new GeneralComm("New Paradigm");
		_controller->bCastOutboundPacket(gc);
		delete gc;
	}
	
	//output
	cout << endl << "Now running paradigm " << _paradigms[_currentParadigm]->getID() << endl << endl;
	
	//change state
	changeState("experiment start");
	
	//chage state further if appropriate
	if(_paradigms[_currentParadigm]->isAutoLoad())
		load();
}

void WaterMaze::changeTrial(int direction)
{
	if(_runningTrial)
		return;
	
	//can we change the trial?
	if(_paradigms[_currentParadigm]->isContinuous())
	{
		//we can.
		//clear geometry.
		clear();
		
		if(_paradigms[_currentParadigm]->isAutoLoad())
			load();
		else
			changeState("experiment start");
		
	}
	else if(_paradigms[_currentParadigm]->trialsRemaining() > 0)
	{
		//we still can.
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

void WaterMaze::reachedDestination()
{
	//disable movement.
	cout << endl << "Destination reached successfully." << endl << endl;
	_paradigms[_currentParadigm]->playSuccess();
	trialEndStateChange();
	
}

void WaterMaze::timeOut()
{
	//stop running trial
	cout << endl << "Time out / Operator force quit occured." << endl << endl;
	_paradigms[_currentParadigm]->playFailure();	 
	trialEndStateChange();
}

void WaterMaze::trialEndStateChange()
{
	//get ending
	_finishing = true;
	_runningTrial = false;
	_howToEnd = _paradigms[_currentParadigm]->getEnding();
	if(NULL != _howToEnd)
	{
		Vec4 color = _howToEnd->getFadeColor();
		color[3] = 1.0;
		PluginHelper::setClearColor(color);
	}

	_endingTimer = PluginHelper::getProgramDuration();	
	changeState("ending trial");			
}

void WaterMaze::writeToLog()
{
	_currPath->writeToLog(_currSubject, _paradigms[_currentParadigm]->getID());
	_currPath->reclaimMemory();
}

void WaterMaze::startStop()
{
	if (_runningTrial)
	{
		timeOut();
	}
	else
	{
		//reset timers. and signal the starting procedure to begin.
		_startTime = PluginHelper::getProgramDuration();
		_androidTimer = PluginHelper::getProgramDuration();
		_startingTimer = PluginHelper::getProgramDuration();
		_howToStart = _paradigms[_currentParadigm]->getStart();
		_starting = true;
		
		changeState("starting trial");
	}
}

string WaterMaze::getState()
{
	return _state;
}

void WaterMaze::changeState(string state)
{
	cout << "changing state to " << state << endl;
	//internal
	this->_state = state;
	//inform devices
	if(ComController::instance()->isMaster())
	{
		StateUpdate* update = new StateUpdate(state);
		_controller->bCastOutboundPacket(update);
		delete update;
	}
}

TrialSetup* WaterMaze::getTrialSetup()
{
	TrialSetup* ts = new TrialSetup();
	ts->trialNo = _paradigms[_currentParadigm]->getTrialNumber();
	ts->timeLimit = _paradigms[_currentParadigm]->getTimeRemaining(0);
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
	TrialSetup* ts = getTrialSetup();
	
	//create a new path.
	_currPath = new Path(ts);
	_paradigms[_currentParadigm]->addPath(_currPath);
		
	//inform android
	if(ComController::instance()->isMaster())
	{
		_controller->bCastOutboundPacket(ts);
	}
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
		//check controller for data.
		bool hasSubject = _controller->hasNewSubjects();
		ComController::instance()->sendSlaves(&hasSubject, sizeof(hasSubject));
		
		if(hasSubject)
		{
			string s = _controller->getNewSubject();
			int size = s.length() + 1;
			char c[size];
			strcpy(c, s.c_str());
			ComController::instance()->sendSlaves(&size, sizeof(size));
			ComController::instance()->sendSlaves(&c, sizeof(c));
			_currSubject = string(c);
			GeneralComm *gc = new GeneralComm("New Subject");
			_controller->bCastOutboundPacket(gc);
			delete gc;
			changeState("experiment start");
		}
    } 
    else 
    {
        bool hasSubject;
		ComController::instance()->readMaster(&hasSubject, sizeof(hasSubject));
		
		if(hasSubject)
		{
			tuple<string, bool> data;
			int size = 0;
			ComController::instance()->readMaster(&size, sizeof(size));
			char c[size];
			ComController::instance()->readMaster(&c, sizeof(c));
			_currSubject = string(c);
			changeState("experiment start");
		}
    }
    
    _currentParadigm = 0;
    
    
}

void WaterMaze::checkMovement()
{
	//movement disabled. 
	float boundSize = ConfigManager::getFloat("Plugin.WaterMaze.CollisionLimit");
	//check collisions.
	//get position matrix
	osg::Vec3 pos = osg::Vec3(0,0,0) * cvr::PluginHelper::getHeadMat() * 
		PluginHelper::getWorldToObjectTransform() * _geoRoot->getInverseMatrix();
	osg::Vec3 prevPos = osg::Vec3(0,0,0) * cvr::PluginHelper::getHeadMat() * 
		Matrixd::inverse(_prevMat) * _geoRoot->getInverseMatrix();
		
	Vec3 diff = pos - prevPos;
	float vel = diff.length() * 60;
	if(vel > maxVel)
	{
		//cout << "Max Velocity: " << vel << endl;
		maxVel = vel;
	}
	osg::Vec3 origin = osg::Vec3(-widthTile, -heightTile, 0);
	
	Vec3 c = pos - origin;	
	
	
	//check bounds of c
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
	
	//Disable movement in z direction
	/*osg::Matrixd mat;
	mat.makeTranslate(osg::Vec3(-startingX, -startingY,0));
	PluginHelper::setObjectMatrix(mat);*/
}

void WaterMaze::handleEnding()
{
	if(_howToEnd != NULL)
	{
		osg::Matrixd mat = PluginHelper::getObjectMatrix();
		if(PluginHelper::getProgramDuration() - _endingTimer >= _howToEnd->getDuration())
		{
			_finishing = false;
			_howToEnd = NULL;
			
			//change state
			endingChangeState();
			
			clear();
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
					checkMovement();
				}
				
				//fade geometry out
				Vec4 color = _filter->getConstantColor();
				color[3] = _howToEnd->getFadeAlpha(PluginHelper::getProgramDuration() - _endingTimer);
				_filter->setConstantColor(color);
			}
		}
		
	}
	else
	{
		endingChangeState();
		
		/// @todo auto advance trial
		clear();
		
	}
}

void WaterMaze::endingChangeState()
{
	if(_currentParadigm == _paradigms.size() - 1) 
	{
		if(_paradigms[_currentParadigm]->isContinuous())
			changeState("experiment end continuous");
		else if(_paradigms[_currentParadigm]->trialsRemaining() > 0)
		{
			if(_paradigms[_currentParadigm]->canEnd())
				changeState("experiment end continuous");
			else
				changeState("trial end");
		}
		else
			changeState("experiment end");
	}
	else
	{
		if(_paradigms[_currentParadigm]->isContinuous())
			changeState("paradigm end continuous");
		else if(_paradigms[_currentParadigm]->trialsRemaining() > 0)
		{
			if(_paradigms[_currentParadigm]->canEnd())
				changeState("paradigm end continuous");
			else
				changeState("trial end");
		}
		else
			changeState("paradigm end");
	}
	
	//write to log
	writeToLog();
}

void WaterMaze::endExperiment()
{
	_currentParadigm = 0;
	//purge all the data
	_paradigms.clear();
	
	//read in data
	std::vector<std::string> paradigmNames;
    ConfigManager::getChildren("Plugin.WaterMaze.Paradigms", paradigmNames);
    for(int i = 0; i < paradigmNames.size(); ++i)
    {
		string path = _dataDir + ConfigManager::getEntry("path", "Plugin.WaterMaze.Paradigms." + paradigmNames[i], "");
		string id = ConfigManager::getEntry("value", "Plugin.WaterMaze.Paradigms." + paradigmNames[i], "");
		Paradigm* p = new Paradigm(id, path);
		_paradigms.push_back(p);
	}
	
	//initialize variables
	_currentParadigm = 0;
	_currPath = NULL;
	changeState("connected");
}

void WaterMaze::handleStart()
{
	//should we still be running through the starting procedure
	if(_howToStart != NULL)
	{
		if(PluginHelper::getProgramDuration() - _startingTimer >= _howToStart->getDuration())
		{
			//Starting procedure is over.
			trialStartStateChange();
		}
		else
		{
			//Handle Geometry rendering
			if(!_howToStart->isGeometryVisible())
			{
				//geometry is not visible.
				Vec4 color = _filter->getConstantColor();
				color[3] = 0.0;
				_filter->setConstantColor(color);
			}
			else
			{
				//fade geometry in
				Vec4 color = _filter->getConstantColor();
				color[3] = _howToStart->getFadeAlpha(PluginHelper::getProgramDuration() - _startingTimer);
				_filter->setConstantColor(color);
				
			}
			
			//Handle Movement
			if(!_howToStart->isMotionAllowed())
			{
				PluginHelper::setObjectMatrix(_prevMat);
			}
			else
			{
				checkMovement();
			}
		}
	}
	else
	{
		//There is no starting procedure.
		
		//fade geometry in
		Vec4 color = _filter->getConstantColor();
		color[3] = 1.0;
		_filter->setConstantColor(color);
		
		//Begin trial
		trialStartStateChange();
	}
}

void WaterMaze::trialStartStateChange()
{
	//Change state variables
	_starting = false;
	_howToStart = NULL;
	_runningTrial = true;
		
	//initialize timer variables.
	_startTime = PluginHelper::getProgramDuration();
	_androidTimer = PluginHelper::getProgramDuration();
	_paradigms[_currentParadigm]->setSoundStartTimer(0);
	
	//create a path
	createPath();

	//change running state
	changeState("running trial");
}

};
