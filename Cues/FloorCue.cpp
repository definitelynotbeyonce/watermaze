#include "FloorCue.h"

FloorCue::FloorCue(string file): Cue(file)
{
	_type = "Floor";

	//read all data from the configuration file
	_subType = _reader.getEntry("Cue.SubType");
	
	if(_subType == "None")
	{
		_showGrid = _reader.getBool("Cue.RedTile");
	}
	
	if(_subType == "Destination Tile")
	{
		_circle = (_text == "Green Circle");
	}	
	
	if(_subType == "Geo")
	{
		_canRender = true;
	}
	else
	{
		_canRender = false;
	}
}

FloorCue::~FloorCue()
{
	
}

void FloorCue::renderGeo(osg::ref_ptr<osg::MatrixTransform> _geoRoot, TrialSetup* ts)
{
	//do nothing this doesnt actually make any rendering happen.
	if(_subType == "Custom Geo")
	{
		renderCustom(_geoRoot, ts);
	}
	if(_subType == "Geo")
	{
		MatrixTransform* cue = new MatrixTransform();
		Geode* geode = new Geode();
		cue->addChild(geode);
		
		//add texture
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
		
		//read in location of cue
		Vec3 vec = _reader.getVec3("Cue.Pos");
		vec[0] = (vec[0] * ts->width * ts->gridSize) - ts->gridSize;
		vec[1] = (vec[1] * ts->length * ts->gridSize) - ts->gridSize;
		vec[2] = vec[2] * ts->wallHeight;
		
		//color is set in the if/else ladder
		Vec4 color;
		
		/// @todo these should not be hard coded
		if(_text == "Yellow Cube")
		{
			//determine scene space size of cue
			vec[2] += ts->gridSize / 8;
			color = Vec4(1.0, 1.0, 0.0, 1.0);	//yellow
			
			//create a cube
			osg::Box * box = new osg::Box(vec, (ts->gridSize / 4), (ts->gridSize / 4), (ts->gridSize / 4));
			ShapeDrawable* sd = new ShapeDrawable(box);
			sd->setColor(color);
			geode->addDrawable(sd);
			geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
			geode->getOrCreateStateSet()->setTextureAttributeAndModes(0,tex,osg::StateAttribute::ON);
			
			//add cue to scene
			cue->addChild(geode);
	
			_geoRoot->addChild(cue);
			
		}
		else if(_text == "Blue Pyramid")
		{
			color = Vec4(0.0, 0.0, 1.0, 1.0);	//blue
			
			//there is no pyramid primative shape.  creating a cylinder instead.
			osg::Cone * cone = new osg::Cone(vec, (ts->gridSize / 8), (ts->gridSize / 4));
			ShapeDrawable* sd = new ShapeDrawable(cone);
			sd->setColor(color);
			geode->addDrawable(sd);
			geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
			geode->getOrCreateStateSet()->setTextureAttributeAndModes(0,tex,osg::StateAttribute::ON);
			
			//add cue to the scene
			cue->addChild(geode);
	
			_geoRoot->addChild(cue);
		} 
	}
}

bool FloorCue::isGrid()
{
	return _showGrid;
}

int FloorCue::isTileCircle()
{
	int num = 0;
	if(_subType == "Destination Tile")
	{
		if(_circle)
			return 1;	//yes
		else
			return 2;	//no
	}
	else
	{
		return 0;		//N/A
	}
}
