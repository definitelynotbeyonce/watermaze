#include "Experiment.h"

//Constructor Destructor
Experiment::Experiment()
{
	time_t now = time(0);
	tm* curr = gmtime(&now);

	_dateTimeStart = *curr;
}
Experiment::~Experiment()
{
	
}

void Experiment::finish()
{
	time_t now = time(0);
	tm* curr = gmtime(&now);

	_dateTimeFinish = *curr;
}
