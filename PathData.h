#ifndef _PATHDATA_H_
#define _PATHDATA_H_

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

#include "EyeData.h"
#include "Controller/OutboundPacket.h"

using namespace std;

class PathData: public OutboundPacket
{
    public:
        PathData(osg::Vec3* pos, float elapsedTime);	//TODO: refactor to implement EyeData
        virtual ~PathData();
        //data recording
        void writeToLog(ofstream &outFile);
        //android reporting
        string getLine();
    protected:
		float _time;
		osg::Vec3* _pos;
		EyeData* _eyeData;
};



#endif
