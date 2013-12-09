#include "Cue.h"

Cue::Cue(string file)
{
	_cueFile = file;
	_reader.loadFile(_cueFile);
	_toggleState = _reader.getBool("Cue.Toggle");
	_text = _reader.getEntry("Cue.Text");
}

Cue::~Cue()
{
	
}

bool Cue::isRenderable()
{
	return _canRender && _toggleState;
}

string Cue::toString()
{
	if(_toggleState)
		return "Button Text:" + _text + "|Toggle:true";
	else
		return "Button Text:" + _text + "|Toggle:false";
}

string Cue::getText()
{
	return _text;
}

void Cue::toggle(bool state)
{
	_toggleState = state;
}

bool Cue::getState()
{
	return _toggleState;
}

string Cue::getType()
{
	return _type;
}

void Cue::renderCustom(osg::ref_ptr<osg::MatrixTransform> _geoRoot, TrialSetup* ts)
{
	//get the file name
	cout << "custom Render" << endl;
	string fileName = _reader.getEntry("value", "Cue.Shapes.Shape0.File", "NULL");
	
	osg::Node* model = osgDB::readNodeFile(ConfigManager::getEntry("Plugin.WaterMaze.DataDir") + fileName);
	
	if(model)
	{
		PositionAttitudeTransform* modelPAT = new PositionAttitudeTransform();
		modelPAT->addChild(model);
		
		//Scale
		float size;
		if(_reader.getInt("Cue.Shapes.Shape0.SizeType") == 1)	//Absolute size
		{
			size =  _reader.getFloat("Cue.Shapes.Shape0.Size", ts->gridSize * 0.5);
		}
		else
		{
			size = ts->gridSize * _reader.getFloat("Cue.Shapes.Shape0.Size", 1.0);
		}
		
		float actualSize = modelPAT->computeBound().radius();
		cout << "object size: " << size << endl;
		cout << "scale factor: " << size/actualSize << endl;
		modelPAT->setScale(Vec3(size/actualSize, size/actualSize, size/actualSize));
		
		//position
		Vec3 pos = _reader.getVec3("Cue.Shapes.Shape0.Pos");
		Vec3 type = _reader.getVec3("Cue.Shapes.Shape0.PosType");
		
		//modify position to be in proper scene space coordinates
		pos[0] *= type[0] == 1.0? 1.0: 1.0 * ts->gridSize * ts->width;
		pos[1] *= type[1] == 1.0? 1.0: 1.0 * ts->gridSize * ts->length;
		pos[2] *= type[2] == 1.0? 1.0: 1.0 * ts->wallHeight;
		cout << "pos: " << "<" << pos.x() << ", " << pos.y() << ", " << pos.z() << ">" << endl;
		Vec3 currCenter = modelPAT->computeBound().center();
		cout << "obj center from file: " << "<" << currCenter.x() << ", " << currCenter.y() << ", " << currCenter.z() << ">" << endl;
		Vec3 displacement = pos - currCenter;
		displacement[0] += ts->zero_zeroX;
		displacement[1] += ts->zero_zeroY;
		cout << "object position: " << "<" << displacement.x() << ", " << displacement.y() << ", " << displacement.z() << ">" << endl;
		modelPAT->setPosition(displacement);
		Vec3 cen = modelPAT->computeBound().center();
		cout << "object position: " << "<" << cen.x() << ", " << cen.y() << ", " << cen.z() << ">" << endl;
		
		//bounding box sphere
		//add the square to the geode
		currCenter *= actualSize/size;
		Sphere* s = new Sphere(currCenter, actualSize);
		ShapeDrawable* sd = new ShapeDrawable(s);
		sd->setColor(Vec4(1.0, 1.0, 1.0, 1.0));
		Geode* geode = new Geode();
		geode->addDrawable(sd);
		geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
		PolygonMode* pm = new PolygonMode;
		pm->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
		geode->getOrCreateStateSet()->setAttributeAndModes(pm, osg::StateAttribute::OVERRIDE| osg::StateAttribute::ON);
		modelPAT->addChild(geode);
		//add to scene
		_geoRoot->addChild(modelPAT);
		
	}
	else
	{
		cout << "File: \"" << ConfigManager::getEntry("Plugin.WaterMaze.DataDir") + fileName << "\" not found." << endl;
	}
	
}

