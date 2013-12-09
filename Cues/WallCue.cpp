#include "WallCue.h"

WallCue::WallCue(string file): Cue(file)
{
	_type = "Wall";
	_subType = _reader.getEntry("Cue.SubType");
	_canRender = true;
}

WallCue::~WallCue()
{
	
}

void WallCue::renderGeo(osg::ref_ptr<osg::MatrixTransform> _geoRoot, TrialSetup* ts)
{
	//route to proper rendering functon
	if(_subType == "None")
	{
		//renderWall(_geoRoot, ts);
	}
	else if(_subType == "Hanging")
	{
		renderHanging(_geoRoot, ts);
	}
	else if(_subType == "Custom")
	{
		renderCustom(_geoRoot, ts);
	}
}

void WallCue::renderWall(osg::ref_ptr<osg::MatrixTransform> _geoRoot, TrialSetup* ts)
{
	//Wall texture
	cout << "Render Wall" << endl;
	std::string wallTex = ConfigManager::getEntry("Plugin.WaterMaze.Textures.Wall");
    osg::Texture2D* tex = new osg::Texture2D();
    osg::ref_ptr<osg::Image> img;

    img = osgDB::readImageFile(ConfigManager::getEntry("Plugin.WaterMaze.DataDir") + wallTex);
    if (img)
    {
        tex->setImage(img);
        tex->setResizeNonPowerOfTwoHint(false);
        tex->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
        tex->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
    }
    
    //Matrix transform containing every shape of the cue.
	MatrixTransform* cue = new MatrixTransform();

	//render each shape from the config file
	vector<string> shapes;
	_reader.getChildren("Cue.Shapes",shapes);
	for(int i = 0; i < shapes.size(); ++i)
	{
		//create geode
		Geode* geode = new Geode();
		cue->addChild(geode);
		
		//get center
		Vec3 vec = _reader.getVec3("Cue.Shapes." + shapes[i] + ".Pos");
		//manip vector.
		vec[0] = (vec[0] * ts->width * ts->gridSize) + ts->zero_zeroX;
		vec[1] = (vec[1] * ts->length * ts->gridSize) + ts->zero_zeroY;
		vec[2] = vec[2] * ts->wallHeight;
		
		//get color
		Vec4 color = _reader.getColor("Cue.Shapes." + shapes[i] + ".Color");
		Vec3 normal = _reader.getVec3("Cue.Shapes." + shapes[i] + ".Normal");
		normal.normalize();
		
		//compute the box
		float x = (normal[0] == 0)?ts->width * ts->gridSize:4;
		float y = (normal[1] == 0)?ts->width * ts->gridSize:4;
		float z = (normal[2] == 0)?ts->wallHeight:4;
		osg::Box * box = new osg::Box(vec, x, y, z);
		
		//add the box
		ShapeDrawable* sd = new ShapeDrawable(box);
		sd->setColor(color);
		geode->addDrawable(sd);
		geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
		geode->getOrCreateStateSet()->setTextureAttributeAndModes(0,tex,osg::StateAttribute::ON);
		
		//add shape to the cue geometry
		cue->addChild(geode);
	}
		
	//add cue to scene
	_geoRoot->addChild(cue);
}

void WallCue::renderHanging(osg::ref_ptr<osg::MatrixTransform> _geoRoot, TrialSetup* ts)
{
	//Matrix transform for cue.	
	cout << "Render Hanging" << endl;
	MatrixTransform* cue = new MatrixTransform();
	Geode* geode = new Geode();
	cue->addChild(geode);
	
	//Uses wall texture for the texture of the hanging.
	std::string wallTex = ConfigManager::getEntry("Plugin.WaterMaze.Textures.Wall");
    osg::Texture2D* tex = new osg::Texture2D();
    osg::ref_ptr<osg::Image> img;

    img = osgDB::readImageFile(ConfigManager::getEntry("Plugin.WaterMaze.DataDir") + wallTex);
    if (img)
    {
        tex->setImage(img);
        tex->setResizeNonPowerOfTwoHint(false);
        tex->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
        tex->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
    }
	
	//get type of shape.
	string type = _reader.getEntry("Cue.Shapes.Shape0.Shape");
	
	//position of shape.
	Vec3 pos = _reader.getVec3("Cue.Shapes.Shape0.Pos"); 
	Vec3 vec = _reader.getVec3("Cue.Shapes.Shape0.Pos");
	cout << "Zero Zero " << ts->zero_zeroX << endl;
	vec[0] = (vec[0] * ts->width * ts->gridSize) + ts->zero_zeroX;
	vec[1] = (vec[1] * ts->length * ts->gridSize) + ts->zero_zeroY;
	vec[2] = vec[2] * ts->wallHeight;
	
	//color of shape
	Vec4 color = _reader.getColor("Cue.Shapes.Shape0.Color");
	
	//use the appropriate rendering for wall hanging type
	if(type == "Square")
	{
		osg::Box * box;
		
		//determine dimensions of box
		if(pos.y() == 0.0 || pos.y() == 1.0)	
		{
			//against left or right wall
			box = new osg::Box(vec, (ts->gridSize / 2), 100, (ts->gridSize / 2));
		}
		else
		{
			//against near or far wall
			box = new osg::Box(vec, 100, (ts->gridSize / 2), (ts->gridSize / 2));
		}
		
		//add the square to the geode
		ShapeDrawable* sd = new ShapeDrawable(box);
		sd->setColor(color);
		geode->addDrawable(sd);
		geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
		geode->getOrCreateStateSet()->setTextureAttributeAndModes(0,tex,osg::StateAttribute::ON);
	}
	else if(type == "Rhombus")
	{
		osg::Box * box;
		Quat axis;
		
		//determine orientation of the rhombus
		if(pos.y() == 0.0 || pos.y() == 1.0)	
		{
			//against left or right wall
			box = new osg::Box(vec, (ts->gridSize / 2), 6, (ts->gridSize / 2));
			axis = Quat((PI/4), Vec3(0.0, 1.0, 0.0));
		}
		else
		{
			//against near or fall wall
			box = new osg::Box(vec, 6, (ts->gridSize / 2), (ts->gridSize / 2));
			axis = Quat((M_PI/4), Vec3(1.0, 0.0, 0.0));
		}
		
		//add the rhombus to the geode
		box->setRotation(axis);
		ShapeDrawable* sd = new ShapeDrawable(box);
		sd->setColor(color);
		geode->addDrawable(sd);
		geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
		geode->getOrCreateStateSet()->setTextureAttributeAndModes(0,tex,osg::StateAttribute::ON);
	}
	else if(type == "Circle")
	{
		osg::Cylinder * circle;
		Quat axis;
		
		//determine orientation of circle.
		if(pos.y() == 0.0 || pos.y() == 1.0)	
		{
			//against left or right wall
			circle = new osg::Cylinder(vec, (ts->gridSize / 4), 6);
			axis = Quat((M_PI/2), Vec3(1.0, 0.0, 0.0));
		}
		else	
		{
			//against near or far wall
			circle = new osg::Cylinder(vec, (ts->gridSize / 4), 6);
			axis = Quat((M_PI/2), Vec3(0.0, 1.0, 0.0));
		}
		
		//add the circle to geode
		circle->setRotation(axis);
		ShapeDrawable* sd = new ShapeDrawable(circle);
		sd->setColor(color);
		geode->addDrawable(sd);
		geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
		geode->getOrCreateStateSet()->setTextureAttributeAndModes(0,tex,osg::StateAttribute::ON);
	}
	else if(type == "Pentagon")	//NOTE: shape is not actually a pentagon
	{
		for(int i = -1; i <= 1; ++i)
		{
			osg::Box * box;
			Vec3 offset;
			
			//determine position/orientation of shape
			if(pos.y() == 0.0 || pos.y() == 1.0)
			{
				//against left or right wall
				offset = Vec3((ts->gridSize / 8) * i, pos.y(), (ts->gridSize / 8) * i);
				box = new osg::Box((vec + offset), (ts->gridSize / 4), 6, (ts->gridSize / 4));
			}
			else
			{
				//against near or far wall
				offset = Vec3(pos.x(), (ts->gridSize / 8) * i, (ts->gridSize / 8) * i);
				box = new osg::Box((vec + offset), 6, (ts->gridSize / 4), (ts->gridSize / 4));
			}
			
			//add the box
			ShapeDrawable* sd = new ShapeDrawable(box);
			sd->setColor(color);
			geode->addDrawable(sd);
			geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
			geode->getOrCreateStateSet()->setTextureAttributeAndModes(0,tex,osg::StateAttribute::ON);
		}
	}
	
	//add geometry to cue
	cue->addChild(geode);
	
	//add cue to scene
	_geoRoot->addChild(cue);
}

