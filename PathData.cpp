#include "PathData.h"

PathData::PathData(osg::Vec3 pos, osg::Vec3 right, osg::Vec3 front, osg::Vec3 up, float elapsedTime)
{
	//position
	_pos = pos;
	
	_eyeData = new EyeData(right, front, up);
	progress = 0;
	_time = elapsedTime;
	
	_packetType = "Data Point";
}

PathData::~PathData()
{
	delete _eyeData;
}

void PathData::writeToLog(ofstream &outFile)
{
	//time
	outFile << _time << "\t";
	//pos
	outFile << _pos.x() << "\t" << _pos.y() << "\t" << _pos.z() << "\t";
	
	//right
	outFile << _eyeData->getRight().x() << "\t" << _eyeData->getRight().y() << "\t" << _eyeData->getRight().z() << "\t";
	//front
	outFile << _eyeData->getFront().x() << "\t" << _eyeData->getFront().y() << "\t" << _eyeData->getFront().z() << "\t";
	//up
	outFile << _eyeData->getUp().x() << "\t" << _eyeData->getUp().y() << "\t" << _eyeData->getUp().z() << "\t";
	
	//EyeData
	
	outFile << endl;
	outFile << flush;
}

string PathData::getLine()
{
	//serialize this object (or as much of it as possible) to a string.
	string returnVal = "";
	bool canRun = true;
	while(canRun)
	{
		char sBuf[256];
		int n;
		switch(progress)
		{
			case 0:	//time
				n = sprintf(sBuf, "Time:%f", _time);
				break;
			case 1:	//x pos
				n = sprintf(sBuf, "X:%f", _pos.x());
				break;
			case 2:	//y pos
				n = sprintf(sBuf, "Y:%f", _pos.y());
				break;
			default:
				canRun = false;
				break;
		}
		if(returnVal.length() + n < 256 && canRun)
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

	return returnVal;
}

osg::Vec3 PathData::getPos()
{
	return _pos;
}

float PathData::getTime()
{
	return _time;
}
