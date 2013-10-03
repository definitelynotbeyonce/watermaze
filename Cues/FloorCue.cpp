#include "FloorCue.h"

FloorCue::FloorCue(string file): Cue(file)
{
	_showGrid = _reader.getBool("Cue.RedTile");
	_type = "Floor";
}

FloorCue::~FloorCue()
{
	
}

void FloorCue::renderGeo(osg::ref_ptr<osg::MatrixTransform> _geoRoot, TrialSetup* ts)
{
	//do nothing this doesnt actually make any rendering happen.
}

bool FloorCue::isGrid()
{
	return _showGrid;
}
