#include "PathData.h"

PathData::PathData(osg::Vec3* pos, float elapsedTime)
{
	//position
	_pos = new osg::Vec3(0,0,0);
	_pos->set(pos->x(),  pos->y(),  pos->z());
	
	//DEBUG:
	//cout << _pos->x() << " " << _pos->y() << " " << _pos->z() << endl;
	
	_eyeData = new EyeData();
	progress = 0;
	_time = elapsedTime;	//TODO
	
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
	//DEBUG:
	cout << _pos->x() << " " << _pos->y() << " " << _pos->z();
	
	//TODO: write eye data
	outFile << endl;
	outFile << flush;
}

string PathData::getLine()
{
	string returnVal = "";
	bool canRun = true;
	while(canRun)
	{
		char sBuf[256];
		int n;
		switch(progress)
		{
			case 0:	//time
				n = sprintf(sBuf, "Time:%i", _time);
				break;
			case 1:
				n = sprintf(sBuf, "X:%f", _pos->x());
				break;
			case 2:
				n = sprintf(sBuf, "Y:%f", _pos->y());
				break;
			default:
				canRun = false;
				break;
		}
		if(returnVal.length() + n < 256)
		{
			if(returnVal != "")
			{
				returnVal.append("|");
			}
			returnVal.append(sBuf, n);
			++progress;
		}
		else
		{
			canRun = false;
		}
	}
	if(returnVal == "")
		returnVal = "NULL";
	cout << "going out: " << returnVal << endl;
	return returnVal;
}
