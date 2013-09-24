#include "Paradigm.h"

//initialize static variables
int Paradigm::_numParadigms = 0;

Paradigm::Paradigm(string paradigmID, string file)
{
	//TODO: massive refactoring!!!
	_paradigmID = paradigmID;
	XMLReader reader;
	if(reader.loadFile(file))
	{
		//control parameters
		_width = reader.getInt("Paradigm.Width");
		_length = reader.getInt("Paradigm.Length");
		_numTrials = reader.getInt("Paradigm.NumTrials");
		_timeOut = reader.getInt("Paradigm.Time");
		_gridLines = reader.getBool("Paradigm.GridLines");
		int read = reader.getInt("Paradigm.StartingType");
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
		read = reader.getInt("Paradigm.FinishType");
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
		
		//cues
		//TODO
		
		//starting position
		bool foundX, foundY;
		int x = reader.getInt("x", "Paradigm.StartingPos", -1, &foundX);
		int y = reader.getInt("y", "Paradigm.StartingPos", -1, &foundY);
		
		if(foundX && foundY)
		{
			_startingPos = x * _width + y;
		}
		else
		{
			_startingPos = -1;
			newStart();
		}
		
		//finish position
		x = reader.getInt("x", "Paradigm.FinishPos", -1, &foundX);
		y = reader.getInt("y", "Paradigm.FinishPos", -1, &foundY);
		if(foundX && foundY)
		{
			cout << "finish x: " << x << " y: " << y << endl;
			_finishPos = x * _width + y;
		}
		else
		{
			_finishPos = -1;
			newFinish();
		}
		
	}
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

string Paradigm::getID()
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



