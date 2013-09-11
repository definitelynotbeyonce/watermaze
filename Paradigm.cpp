#include "Paradigm.h"

//initialize static variables
int Paradigm::_numParadigms = 0;

Paradigm::Paradigm()
{
	//TODO: Eventually the _paradigmID will be passed in from the constructor
	_paradigmID = _numParadigms++;	
	
	char buf[50];
	sprintf(buf, "Plugin.WaterMaze.Paradigms.%i", _paradigmID);
	std::string path = std::string(buf);
	
	//read in primitive types
	_width = cvr::ConfigManager::getInt("Width", path);
	_length = cvr::ConfigManager::getInt("Length", path);
	_numTrials = cvr::ConfigManager::getInt("NumTrials", path);
	_timeOut = cvr::ConfigManager::getInt("Time", path);
	_gridLines = cvr::ConfigManager::getBool("GridLines", path);
	
	//read in nonprimitives
	int read = cvr::ConfigManager::getInt("StartingType", path);
	switch(read)
	{
		case 0:
			_startingPosType = Constant;
			break;
		case 1:
			_startingPosType = Random;
			break;
		default:
			_startingPosType = Constant;
			break;
	}
	//_startingPosType;
	//_finishPosType
	read = cvr::ConfigManager::getInt("FinishType", path);
	switch(read)
	{
		case 0:
			_finishPosType = Constant;
			break;
		case 1:
			_finishPosType = Random;
			break;
		default:
			_finishPosType = Constant;
			break;
	}
	
	//generated values
	_startingPos = -1;
	_finishPos = -1;
	newStart();
	newFinish();
	//_staringLookAt;
}

Paradigm::~Paradigm()
{
    for (int i = 0; i < _trials.size(); ++i)
    {
		delete _trials[i];
	}
}

//modifier methods
void Paradigm::addPath(Path* p)
{
	cout << "adding path" << endl;
	_trials.push_back(p);
}
void Paradigm::addTrial()
{
	_numTrials += 1;
}
void Paradigm::setStartingPos()
{
	if (_startingPosType == Random)
	{
		newStart();
	}
}

void Paradigm::setFinishPos()
{
	if (_finishPosType == Random)
	{
		newFinish();
	}
}

//access methods
int Paradigm::getStartingPos()
{
	return _startingPos;
}

int Paradigm::getFinishPos()
{
	return _finishPos;
}

int Paradigm::trialsRemaining()
{
	return _numTrials - _trials.size();
}

bool Paradigm::isContinuous()
{
	return _numTrials < 0;
}

int Paradigm::getWidth()
{
	return _width;
}

int Paradigm::getLength()
{
	return _length;
}

int Paradigm::getTileSize()
{
	return _tileSize;
}

int Paradigm::getTrialNumber()
{
	return _trials.size() + 1;
}

float Paradigm::getTimeRemaining(float duration)
{
	return _timeOut - duration;
}

int Paradigm::getID()
{
	return _paradigmID;
}

//data recording
void Paradigm::writeToLog(ofstream &outFile)
{
	//TODO:
	outFile << "<" << _paradigmID << ">" << endl;
	outFile << _trials.size() << endl;
	
	for(int i = 0; i < _trials.size(); ++i)
	{
		outFile << "<Trial" << i << ">" << endl;
		_trials[i]->writeToLog(outFile);
		outFile << "</Trial" << i << ">" << endl;
	}
	
	outFile << "</" << _paradigmID << ">" << endl;
	outFile << flush;
}

//internal utilities
void Paradigm::newStart()
{
	//make sure the new start is different from the old one
	int newPos;
	do
	{
		newPos = rand() % (_width * _length);
	}while(newPos == _startingPos || newPos == _finishPos);
	
	_startingPos = newPos;
}

void Paradigm::newFinish()
{
	//make sure the new start is different from the old one
	int newPos;
	do
	{
		newPos = rand() % (_width * _length);
	}while(newPos == _startingPos || newPos == _finishPos);
	
	_finishPos = newPos;
}



