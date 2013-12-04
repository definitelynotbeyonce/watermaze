#include "Path.h"

Path::Path()
{
	_prevPathData = NULL;
}

Path::Path(TrialSetup* ts)
{
	_setup = ts;
	_success = false;
	_distance = 0;
	_prevPathData = NULL;
}

Path::~Path()
{
	reclaimMemory();
	delete _setup;
}

float Path::getTotalTime()
{
	float first = _path.front()->getTime();
	float last = _path.back()->getTime();
	return last - first;
}

PathData* Path::getLatest()
{
	return _prevPathData;
}

void Path::addStep(float timeElapsed, PathData* pd)
{
	if(_prevPathData != NULL)
	{
		osg::Vec3 displacement = pd->getPos() - _prevPathData->getPos();
		_distance += displacement.length();
	}
	
	_prevPathData = pd;	
	_path.push_back(pd);
}

//data recording
void Path::writeToLog(string subjectID, string paradigmID)
{
	writeTrialDataToLog(subjectID, paradigmID);
	writeTrialInfoToLog(subjectID, paradigmID);
}

void Path::writeTrialDataToLog(string subjectID, string paradigmID)
{
	//get file name.
	stringstream ss (stringstream::in | stringstream::out);
	string dataDir = ConfigManager::getEntry("Plugin.WaterMaze.DataDir");
	ss << dataDir << "Logs/" << subjectID << "_data_" << paradigmID << "_" << _setup->trialNo;
	string fileName = ss.str();
	ss.flush();
	
	//make sure this log file is unique
	int iAppend = 1;
	string result = fileName + ".txt";
	ifstream ifile(result);
	
	//append (x) to the file name.
	while(ifile)
	{
		cout << "file " << result << " existed" << endl;
		ifile.close();
		stringstream filess (stringstream::in | stringstream::out);
		filess << fileName << "(" << iAppend << ")" << ".txt";
		result = filess.str();
		ifile.open(result);
		++iAppend;
	}
	
	//create the log file
	cout << "writing log file:" << result << endl;
	ofstream outFile;
	outFile.open(result, ios::app);
	
	//write contents to log file
	if(outFile)
	{
		for(int i = 0; i < _path.size(); ++i)
		{
			_path[i]->writeToLog(outFile);
		}
		outFile.close();
	}
	else
	{
		cout << "WaterMaze: Unable to open log file " << fileName << endl;
	}
}

void Path::writeTrialInfoToLog(string subjectID, string paradigmID)
{
	//get file name.
	stringstream ss (stringstream::in | stringstream::out);
	string dataDir = ConfigManager::getEntry("Plugin.WaterMaze.DataDir");
	ss << dataDir << "Logs/" << subjectID << "_info_" << paradigmID << "_" << _setup->trialNo;
	string fileName = ss.str();
	
	ss.flush();
	
	//make sure this log file is unique
	int iAppend = 1;
	ifstream ifile(fileName + ".txt");
	string result = fileName + ".txt";
	cout << "starting with " << result << endl;
	
	//append (x) to the file name.
	while(ifile)
	{
		cout << "file " << result << " existed" << endl;
		ifile.close();
		stringstream filess (stringstream::in | stringstream::out);
		filess << fileName << "(" << iAppend << ")" << ".txt";
		result = filess.str();
		ifile.open(result);
		++iAppend;
	}
	
	//create the log file
	cout << "writing log file:" << result << endl;
	ofstream outFile;
	outFile.open(result, ios::app);

	//write contents to log file
	if (outFile) 
	{		
		//grid coordinate starting position
		int x = _setup->startingPos / _setup->length;
		int y = _setup->startingPos % _setup->length;
		outFile << "startingXY\t" << x << "," << y << endl;
		
		//scene space starting position
		Vec3 pos = _path.front()->getPos();
		outFile << "startingXYZ\t" << pos.x() << "," << pos.y() << "," << pos.z() <<  endl;
		//stop position
		pos = _path.back()->getPos();
		outFile << "finalXYZ\t" << pos.x() << "," << pos.y() << "," << pos.z() <<  endl;
		//target position
		x = _setup->finishPos / _setup->length;
		y = _setup->finishPos % _setup->length;
		outFile << "targetXY\t" << x << "," << y << endl;
		
		//width
		outFile << "width\t" << _setup->width << endl;
		//length
		outFile << "length\t" << _setup->length << endl;
		//trial duration
		outFile << "duration\t" << _path.back()->getTime() << endl;
		//distance traveled
		outFile << "distance\t" << _distance << endl; 
		//grid size
		outFile << "gridSize\t" << _setup->gridSize << endl;
		//zero zero
		outFile << "Origin \t" << _setup->zero_zeroX << "," << _setup->zero_zeroY << endl;
		
		outFile << flush;
		outFile.close();
	}
	else
	{
		cout << "WaterMaze: Unable to open log file " << fileName << endl;
	}
}

void Path::succeeded()
{
	_success = true;
}

void Path::reclaimMemory()
{
	//delte all the pathdata without deleting this object
	for(int i = 0; i < _path.size(); ++i)
	{
		delete _path[i];
	}
}



