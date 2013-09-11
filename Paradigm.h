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

#include <vector>

#include <iostream>
#include "Path.h"
#include "Cue.h"

using namespace std;

enum PositionType { Constant = 0, Random };

class Paradigm
{
    public:
        Paradigm();
        virtual ~Paradigm();
        
        //modifier methods
        void addPath(Path* p);
        void addTrial();
        void setStartingPos();
        void setFinishPos();
        
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
        int getID();
        
        //data recording methods
        void writeToLog(ofstream &outFile);
    protected:
		//internal utilities
		void newStart();
        void newFinish();
		
		//paradigm identifier values
		int _paradigmID;
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
		
};






#endif
