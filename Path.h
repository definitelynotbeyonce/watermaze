#ifndef _PATH_H_
#define _PATH_H_

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

#include <string.h>
#include <vector>
#include <map>

#include <iostream>
#include <stdio.h>

#include "PathData.h"

using namespace std;

class Path
{
    public:
        Path();
        Path(int start, int finish);
        virtual ~Path();
        //modifier methods
        void addStep(float timeElapsed, PathData* pd);
        void succeeded();
        
        //access methods
        float getTotalTime();
        
        //data recording
        void writeToLog(ofstream &outFile);
        
    protected:
		std::map<float, PathData*> _path;	//time, location TODO: refactor Vec3 to be PathData
		int _startingPos;
		int _finishPos;
		bool _success;
};



#endif
