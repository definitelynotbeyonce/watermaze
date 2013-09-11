#ifndef _EXPERIMENT_H_
#define _EXPERIMENT_H_

//osg includes

//cvr includes

//std includes
#include <ctime>
#include <vector>

//user defined includes
#include "Paradigm.h"

using namespace std;

class Experiment
{
	public:
		Experiment();
		virtual ~Experiment();
		void finish();
	protected:
		string _subjectID;
		tm _dateTimeStart;
		tm _dateTimeFinish;
		vector<Paradigm*> _paradigms;
		
		
};

#endif
