#include "Paradigm.h"

Paradigm::Paradigm(string paradigmID, string file)
{
	_paradigmID = paradigmID;
	XMLReader reader;
	if(reader.loadFile(file))
	{
		//control parameters
		_width = reader.getInt("Paradigm.Width");
		_length = reader.getInt("Paradigm.Length");
		_numTrials = reader.getInt("Paradigm.NumTrials");
		_minTrials = reader.getInt("Paradigm.MinTrials");
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
		_autoLoad = reader.getBool("Paradigm.AutoLoad");
		
		//cues
		vector<string> cues;
		reader.getChildren("Paradigm.Cues", cues);
		
		for(int i = 0; i < cues.size(); ++ i)
		{
			string type = reader.getEntry("type", "Paradigm.Cues." + cues[i], "");
			string fileName = reader.getEntry("value", "Paradigm.Cues." + cues[i], "");
			Cue* cue = newCue(type, fileName);
			if(cue != NULL)
			{
				_cues.push_back(cue);
			}
		}
		
		//starting position
		bool foundX, foundY;
		int x = reader.getInt("x", "Paradigm.StartingPos", -1, &foundX);
		int y = reader.getInt("y", "Paradigm.StartingPos", -1, &foundY);
		
		if(foundX && foundY)
		{
			_startingPos = x * _length + y;
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
			_finishPos = x * _length + y;
		}
		else
		{
			_finishPos = -1;
			newFinish();
		}
		
	}
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

//internal utilities
void Paradigm::newStart()
{	
	//these variables are used to determine validity of new start position
	int newPos, nX, nY;
	int sX = _startingPos / _length;
	int sY = _startingPos % _length;
	int fX = _finishPos / _length;
	int fY = _finishPos % _length;
	
	bool valid = false;
	do
	{
		//use random number generator to get a new position
		newPos = rand() % (_width * _length);
		nX = newPos / _length;
		nY = newPos % _length;
		
		//it is far enough away from the finish position?
		valid = abs(nX - fX) > 1 && abs(nY - fY) > 1 && (abs(nX - fX) + abs(nY - fY)) > (_width / 3);
		if(_finishPos == -1)	//allow for any starting position from the start.
			valid = true;
			
		if(valid)
		{
			//and far enough away from the previous start?
			valid = abs(nX - sX) > 1 && abs(nY - sY) > 1 && (abs(nX - sX) + abs(nY - sY)) > (_width / 3);
			if(_startingPos == -1)	//if we didnt have a starting position before anything is valid
				valid = true;
		}
		
		
	}while(!valid);
	
	_startingPos = newPos;
}

void Paradigm::newFinish()
{
	//used for validity testing
	int newPos, nX, nY;
	int sX = _startingPos / _length;
	int sY = _startingPos % _length;
	int fX = _finishPos / _length;
	int fY = _finishPos % _length;
	
	bool valid = false;
	do
	{
		newPos = rand() % (_width * _length);
		nX = newPos / _length;
		nY = newPos % _length;
		
		//it is far enough away from the starting position?
		valid = abs(nX - sX) > 1 && abs(nY - sY) > 1 && (abs(nX - sX) + abs(nY - sY)) > (_width / 3);
		if(_startingPos == -1)
			valid = true;
				
		if(valid)
		{
			//and far enough away from the previous finish?
			valid = abs(nX - fX) > 1 && abs(nY - fY) > 1 && (abs(nX - fX) + abs(nY - fY)) > (_width / 3);
			if(_finishPos == -1)	//if no previous finish position any position is valid
				valid = true;
		}
	}while(!valid);
	
	_finishPos = newPos;
}

Cue* Paradigm::newCue(string type, string file)
{
	file = ConfigManager::getEntry("Plugin.WaterMaze.DataDir") + file;
	if(type == "Wall")
	{
		return new WallCue(file);
	}
	else if(type == "Floor")
	{
		return new FloorCue(file);
	}
	else if(type == "End")
	{
		return new EndCue(file);
	}
	else if(type == "Start")
	{
		return new StartCue(file);
	}
	else if(type == "Auditory")
	{
		AuditoryCue* ac = new AuditoryCue(file);
		_soundCues.push_back(ac);
		
		return ac;
	}
	else
	{
		return NULL;
	}
}

void Paradigm::renderCues(osg::ref_ptr<osg::MatrixTransform> _geoRoot, TrialSetup* ts)
{
	for(int i = 0; i < _cues.size(); ++i)
	{
		if(_cues[i]->isRenderable())
		{
			_cues[i]->renderGeo(_geoRoot, ts);
		}
	}
}

vector<Cue*> Paradigm::getCues()
{
	return _cues;
}

void Paradigm::toggle(tuple<string, bool> t)
{
	cout << "Toggling cue: " << get<0>(t) << " value: " << get<1>(t) << endl;
	
	//get the cue
	Cue* c = NULL;
	for(int i = 0; i < _cues.size(); ++i)
	{
		if(get<0>(t) == _cues[i]->getText())
		{
			c = _cues[i];
			break;
		}
	}
	
	if(c != NULL)	//did we find that cue?
	{
		//then toggle it.	
		c->toggle(get<1>(t));
	}
}

bool Paradigm::isAutoLoad()
{
	return _autoLoad;
}

bool Paradigm::isColoredGrid()
{
	for(int i = 0; i < _cues.size(); ++i)
	{
		if(_cues[i]->getText() == "Grid Coloring")
		{
			return _cues[i]->getState();
		}
	}
	
	return false;
}

EndCue* Paradigm::getEnding()
{
	for(int i = 0; i < _cues.size(); ++i)
	{
		if(_cues[i]->getType() == "End")
		{
			return dynamic_cast<EndCue*>(_cues[i]);
		}
	}
	return NULL;
}

StartCue* Paradigm::getStart()
{
	for(int i = 0; i < _cues.size(); ++i)
	{
		if(_cues[i]->getType() == "Start")
		{
			return dynamic_cast<StartCue*>(_cues[i]);
		}
	}
	return NULL;
}

bool Paradigm::isTileCircle()
{
	for(int i = 0; i < _cues.size(); ++i)
	{
		if(_cues[i]->getType() == "Floor")
		{
			int num = (dynamic_cast<FloorCue*>(_cues[i]))->isTileCircle();
			if(num > 0)
				return num == 1;
		}
	}
	
	return false;
}

bool Paradigm::canEnd()
{
	int trialCount = _trials.size();
	return trialCount >= _minTrials;
}

void Paradigm::playSuccess()
{
	for(int i = 0; i < _soundCues.size(); ++i)
	{
		if(_soundCues[i]->isSuccess())
		{
			_soundCues[i]->playSound();
		}
	}
}

void Paradigm::playFailure()
{
	for(int i = 0; i < _soundCues.size(); ++i)
	{
		if(_soundCues[i]->isFailure())
		{
			_soundCues[i]->playSound();
		}
	}
}

void Paradigm::playSoundCues(float time)
{
	for(int i = 0; i < _soundCues.size(); ++i)
	{
		if(_soundCues[i]->isContinuous() || _soundCues[i]->isPlayOnce())
		{
			_soundCues[i]->playSound(time);
		}
	}
}

void Paradigm::playSoundCues(Vec3 pos)
{
	for(int i = 0; i < _soundCues.size(); ++i)
	{
		if(_soundCues[i]->isMovement())
		{
			_soundCues[i]->playSound(pos);
		}
	}
}

void Paradigm::setSoundStartTimer(float time)
{
	for(int i = 0; i < _soundCues.size(); ++i)
	{
		_soundCues[i]->setStartTimer(time);
	}
}

