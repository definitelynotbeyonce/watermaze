#include "StateUpdate.h"

StateUpdate::StateUpdate()
{
	_packetType = "State Update";
	_state = "";
	_sent = false;
}

StateUpdate::StateUpdate(string state)
{
	_packetType = "State Update";
	_state = state;
	_sent = false;
}

StateUpdate::~StateUpdate()
{
	
}
string StateUpdate::getLine()
{
	if(_sent)
	{
		return "NULL";
	}
	else
	{
		_sent = true;
		return _state;
	}
}


