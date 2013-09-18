#include "TrialSetup.h"

TrialSetup::TrialSetup()
{
	_packetType = "Trial Setup";
	progress = 0;
}
TrialSetup::~TrialSetup()
{
	
}

string TrialSetup::getLine()
{
	char pipe = '|';
	string returnVal = "";
	bool canRun = true;
	while(canRun)
	{
		char sBuf[256];
		int n;
		switch(progress)
		{
			case 0:	//trialNo
				n = sprintf(sBuf, "TrialNo:%i", trialNo);
				break;
			case 1: //timeLimit
				n = sprintf(sBuf, "TimeLimit:%i", timeLimit);
				break;
			case 2:	//gridSize
				n = sprintf(sBuf, "GridSize:%f", gridSize);
				break;
			case 3:	//length
				n = sprintf(sBuf, "Length:%i", length);
				break;
			case 4: //width
				n = sprintf(sBuf, "Width:%i", width);
				break;
			case 5:	//zero zero
				n = sprintf(sBuf, "ZeroZero:%f_%f", zero_zeroX, zero_zeroY);
				break;
			case 6: //StartingPos
				n = sprintf(sBuf, "startingPos:%i", startingPos);
				break;
			case 7:	//FinishPos
				n = sprintf(sBuf, "FinishPos:%i", finishPos);
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
	return returnVal;

}


