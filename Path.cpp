#include "Path.h"

Path::Path()
{
	//cout << "creating a new path" << endl;
}

Path::Path(int start, int finish)
{
	_startingPos = start;
	_finishPos = finish;
	_success = false;
}

Path::~Path()
{
	//TODO: refactor
	std::map<float, PathData*>::iterator it;
    for (it = _path.begin(); it != _path.end(); ++it)
    {
		delete it->second;
	}
}

float Path::getTotalTime()
{
	float first = _path.begin()->first;
	float last = _path.end()->first;
	return last - first;
}

void Path::addStep(float timeElapsed, PathData* pd)
{
	_path.insert(std::pair<float, PathData*>(timeElapsed, pd));
}

void Path::writeToLog(ofstream &outFile)
{
	//TODO: refactor
	std::map<float, PathData*>::iterator it;
    for (it = _path.begin(); it != _path.end(); ++it)
    {
		outFile << it->first << "_";
		it->second->writeToLog(outFile);
	}
	//TODO: success or failure
	outFile << flush;
}

void Path::succeeded()
{
	_success = true;
}



