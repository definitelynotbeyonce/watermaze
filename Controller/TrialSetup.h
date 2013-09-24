#ifndef _TRIALSETUP_H
#define _TRIALSETUP_H

#include <string>
#include "OutboundPacket.h"

using namespace std;

class TrialSetup: public OutboundPacket
{
    public:
		TrialSetup();
        virtual ~TrialSetup();
		string getLine();
		
		int trialNo;
		float timeLimit;
		float gridSize;
		int length;
		int width;
		float zero_zeroX;
		float zero_zeroY;
		int startingPos;
		int finishPos;
		
		//TODO: fields that need to be added to the network parsing.
		float wallHeight;
	protected:
		
};

#endif

