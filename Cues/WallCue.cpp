#include "WallCue.h"

using namespace std;
using namespace osg;
using namespace cvr;

WallCue::WallCue(string file): Cue(file)
{
	_type = "Wall";
}

WallCue::~WallCue()
{
	
}

void WallCue::renderGeo(osg::ref_ptr<osg::MatrixTransform> _geoRoot, TrialSetup* ts)
{
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
    
	MatrixTransform* cue = new MatrixTransform();

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
		vec[0] = (vec[0] * ts->width * ts->gridSize) - ts->gridSize;
		vec[1] = (vec[1] * ts->length * ts->gridSize) - ts->gridSize;
		vec[2] = vec[2] * ts->wallHeight;
		//cout << "x: " << vec.x() << " y: " << vec.y() << " z: " << vec.z() << endl;
		
		//get color
		Vec4 color = _reader.getColor("Cue.Shapes." + shapes[i] + ".Color");
		Vec3 normal = _reader.getVec3("Cue.Shapes." + shapes[i] + ".Normal");
		normal.normalize();
		
		//compute the box
		float x = (normal[0] == 0)?ts->width * ts->gridSize:4;
		float y = (normal[1] == 0)?ts->width * ts->gridSize:4;
		float z = (normal[2] == 0)?ts->wallHeight:4;
		//cout << "x: " << x << " y: " << y << " z: " << z << endl;
		osg::Box * box = new osg::Box(vec, x, y, z);
		
		//add the box
		ShapeDrawable* sd = new ShapeDrawable(box);
		sd->setColor(color);
		geode->addDrawable(sd);
		geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
		geode->getOrCreateStateSet()->setTextureAttributeAndModes(0,tex,osg::StateAttribute::ON);
		
		cue->addChild(geode);
	}
		
		

	
	_geoRoot->addChild(cue);
}
