#ifndef _STATEUPDATE_H
#define _STATEUPDATE_H

#include <string>
#include "OutboundPacket.h"

using namespace std;

class StateUpdate: public OutboundPacket
{
    public:
		StateUpdate();
		StateUpdate(string state);
        virtual ~StateUpdate();
		string getLine();
		
	protected:
		string _state;
		bool _sent;
};

#endif

