#include "Cue.h"

Cue::Cue(string file)
{
	_cueFile = file;
	_reader.loadFile(_cueFile);
	_toggleState = _reader.getBool("Cue.Toggle");
	_text = _reader.getEntry("Cue.Text");
}

Cue::~Cue()
{
	
}

bool Cue::isRenderable()
{
	return _type == "Wall" && _toggleState;
}

string Cue::toString()
{
	if(_toggleState)
		return "Button Text:" + _text + "|Toggle:true";
	else
		return "Button Text:" + _text + "|Toggle:false";
}

string Cue::getText()
{
	return _text;
}

void Cue::toggle(bool state)
{
	_toggleState = state;
}

bool Cue::getState()
{
	return _toggleState;
}

string Cue::getType()
{
	return _type;
}
