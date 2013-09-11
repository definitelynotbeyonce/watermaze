#include "PathData.h"

PathData::PathData(osg::Vec3* pos)
{
	//position
	_pos = new osg::Vec3(0,0,0);
	_pos->set(pos->x(),  pos->y(),  pos->z());
	
	_eyeData = new EyeData();
	_progress = 0;
	_time = 0;	//TODO
	
	_packetType = "Data Point";
}

PathData::~PathData()
{
	delete _pos;
	delete _eyeData;
}

void PathData::writeToLog(ofstream &outFile)
{
	outFile << _pos->x() << "_" << _pos->y() << "_" << _pos->z();
	//TODO: write eye data
	outFile << endl;
	outFile << flush;
}

string PathData::getLine()
{
	string returnVal = "b";
	bool canRun = true;
	while(canRun)
	{
		char sBuf[256];
		int n;
		switch(_progress)
		{
			case 0:	//pos
				n = sprintf(sBuf, "Time:%i", time);
				break;
			/* TODO: is this needed?	case 1: //Time
				n = sprintf("Time:%f", _time);
				break;*/
			//TODO: implement eye data etc,
			default:
				return "NULL";
		}
		if(returnVal.length() + n < 256)
		{
			returnVal.append(sBuf, n);
			++_progress;
		}
		else
		{
			canRun = false;
		}
	}
}
