#ifndef _PARADIGM_H_
#define _PARADIGM_H_

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

#include <cvrKernel/PluginHelper.h>
#include <cvrConfig/ConfigManager.h>
#include <cvrConfig/XMLReader.h>

#include <vector>
#include <iostream>
#include <tuple>

#include "Path.h"
#include "Cue.h"
#include "Cues/WallCue.h"
#include "Cues/FloorCue.h"
#include "Cues/EndCue.h"

using namespace std;
using namespace cvr;

enum PositionType { Constant = 0, Random };

class Paradigm
{
    public:
        Paradigm(string paradigmID, string file);
        virtual ~Paradigm();
        
        //modifier methods
        void addPath(Path* p);
        void addTrial();
        void setStartingPos();
        void setFinishPos();
        void toggle(tuple<string, bool> t);
        
        //access methods
        int getStartingPos();
        int getFinishPos();
        int trialsRemaining();
        bool isContinuous();
        int getWidth();
        int getLength();
        int getTileSize();
        int getTrialNumber();
        float getTimeRemaining(float duration);
        string getID();
        vector<Cue*> getCues();
        bool isAutoLoad();
        bool isColoredGrid();
        //float getWidthGrid();
        //float getHeightGrid();
        EndCue* getEnding();
        
        //other
        void renderCues(osg::ref_ptr<osg::MatrixTransform> _geoRoot, TrialSetup* ts);
        
        //data recording methods
        void writeToLog(ofstream &outFile);
    protected:
		//internal utilities
		void newStart();
        void newFinish();
        Cue* newCue(string type, string file);
		
		//paradigm identifier values
		string _paradigmID;
		static int _numParadigms;
		
		//cotrol parameters
		int _length;	
		int _width;
		int _tileSize;				//not being implimented yet
		int _wallHeight;			//not being implimented yet
		std::vector<Cue*> _cues;	//not going to be implimented yet
		PositionType _startingPosType;
		PositionType _finishPosType;
		int _startingPos;
		osg::Vec3 _startingLookAt;	//TODO: use this
		int _finishPos;
		int _numTrials;
		int _timeOut;				
		bool _gridLines;			//NOTE: is this used?
		
		//data recording values
		std::vector<Path*> _trials;	//id, path
		
		bool _autoLoad;
		
};






#endif
