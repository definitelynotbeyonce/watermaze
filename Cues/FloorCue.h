#ifndef _FLOORCUE_H_
#define _FLOORCUE_H_

//osg includes

//cvr includes

//std includes
#include <string>

//user defined includes
#include "../Cue.h"

using namespace std;

class FloorCue: public Cue
{
	public:
		FloorCue(string file);
		virtual ~FloorCue();
		void gridLines();
		void renderGeo(osg::ref_ptr<osg::MatrixTransform> _geoRoot, TrialSetup* ts);
		bool isGrid();
	protected:
		bool _showGrid;
		
};

#endif
