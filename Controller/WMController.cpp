#include "WMController.h"

using namespace std;
using namespace cvr;

namespace WaterMaze
{
//Constructor / Destructor
WMController::WMController()
{
	_portNo = 12012;
	cout << "listening on port: " << 12012 << endl;
	_incoming = new MultiListenSocket(_portNo);
	if(!_incoming->setup())
		cout << "could not start" << endl;
	_wm = NULL;
}

WMController::WMController(WaterMaze* wm)
{
	_portNo = 12012;
	cout << "listening on port: " << 12012 << endl;
	_incoming = new MultiListenSocket(_portNo);
	if(!_incoming->setup())
		cout << "could not start" << endl;
	_wm = wm;
}

WMController::~WMController()
{
	delete _incoming;
}

//inbound packet functions
void WMController::getConnections()
{
	CVRSocket* s = _incoming->accept();
	if(s != NULL)
	{
		cout << "New socket" << endl;
		s->setNoDelay(true);
		_socketList.push_back(s);
	}
}

void WMController::getData()	
{
	if(!_socketList.size())
    {
        return;
    }

    int maxfd = 0;

    fd_set socketsetR;
    FD_ZERO(&socketsetR);

    for(int i = 0; i < _socketList.size(); i++)
    {
        FD_SET((unsigned int)_socketList[i]->getSocketFD(),&socketsetR);
        if(_socketList[i]->getSocketFD() > maxfd)
        {
            maxfd = _socketList[i]->getSocketFD();
        }
    }

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;

    select(maxfd+1,&socketsetR,NULL,NULL,&tv);

    for(std::vector<CVRSocket*>::iterator it = _socketList.begin(); it != _socketList.end(); )
    {
        if(FD_ISSET((*it)->getSocketFD(),&socketsetR))
        {
            if(!processSocketInput(*it))
            {
                cout << "Removing Control socket." << std::endl;
                delete *it;
                it = _socketList.erase(it);
            }
            else
            {
                it++;
            }
        }
        else
        {
            it++;
        }
    }
}

//outbound packet functions
void WMController::bCastOutboundPacket(OutboundPacket* obp)
{
	int n = _socketList.size();	//LCV used for omp parallelization
	
	//1
	tuple<char*, int> t1 = prepJavaString('a', obp->getType());
	#pragma omp parallel for schedule(dynamic)
	for(int i = 0; i < n; ++i)
	{
		_socketList[i]->send(get<0>(t1), get<1>(t1));
	}
	
	//2
	string s2;
	while((s2 = obp->getLine()) != "NULL")
	{
		tuple<char*, int> t2 = prepJavaString('b', s2);
		#pragma omp parallel for schedule(dynamic)
		for(int i = 0; i < n; ++i)
		{
			_socketList[i]->send(get<0>(t2), get<1>(t2));
		}
	}
	
	//3
	char c3[2];
	c3[0] = 'c';
	c3[1] = '\n';	
	#pragma omp parallel for schedule(dynamic)
	for(int i = 0; i < n; ++i)
	{
		_socketList[i]->send(c3, 2);
	}
}




//process input
bool WMController::processSocketInput(cvr::CVRSocket * socket)
{
	InboundPacket* packet = NULL;
	int result = processPacket(1, socket, packet);
	if(result < 0)
		return false;
	
	result = 2;
	while(result == 2)
	{
		result = processPacket(2,socket, packet);
		if(result < 0)
		{
			return false;
		}
	}
	
	//respond
	if(packet->getType() == "State Request")
		sendState(socket);
	else if(packet->getType() == "Trial Setup Request")
		singleDeviceOutput(socket, _wm->getTrialSetup());
	else if(packet->getType() == "Cue List Request")
		singleDeviceOutput(socket, _wm->getCueList());
		
	//push the packet into the queue
	_packets.add(packet);
	
	return true;
}



int WMController::processPacket(int stage, CVRSocket * socket, InboundPacket* &p)
{
	//read stage number
	int num[2];
    if(!socket->recv(num,sizeof(int) * 2))
	{
		return -1;
	}
	//get the packet
	char buf[num[1]];
    if(!socket->recv(buf,num[1]))
    {
        return -1;
	}
    
	//turn last line feed into a null terminator. convert to string
	buf[num[1] - 1] = (char)0;
	//cout << num[0] << " echo: " << buf << endl;
	
	//process packet
	switch(stage)
	{
		case 1:
			p = processType(string(buf), socket);
			break;
		case 2:
			if(num[0] == 2)
			{
				p->addLine(string(buf));
			}
			break;
		default:
			break;
	}
	
	return num[0];	//return the stage as determined by the incoming packet used as an LCV	
}

InboundPacket* WMController::processType(string type, CVRSocket* socket)
{
	if(type == "Command"){
		return (new Command());
	}
	else if(type == "State Request"){
		return (new StateRequest(socket));
	}
	else if(type == "Trial Setup Request")
	{
		return (new TrialSetupRequest());
	}
	else if(type == "Cue List Request")
	{
		return (new CueListRequest());
	}
	else if(type == "Cue Toggle")
	{
		return (new CueToggle());
	}
	else if(type == "New Subject")
	{
		return (new NewSubject());
	}
	else{
		//this shouldnt happen
		cout << "error unknown packet type: " << type << endl;
		return NULL;
	}
}

void WMController::sendState(CVRSocket* destination)
{
	cout << "sending state" << endl;
	StateUpdate* su = new StateUpdate(_wm->getState());
	singleDeviceOutput(destination, su);
	delete su;
}

// output
void WMController::singleDeviceOutput(CVRSocket* socket, OutboundPacket* obp)
{
	cout << "sending outbound packet to single device" << endl;
	
	//1
	tuple<char*, int> t1 = prepJavaString('a', obp->getType());
	socket->send(get<0>(t1), get<1>(t1));
	
	//2
	string s2;
	while((s2 = obp->getLine()) != "NULL")
	{
		tuple<char*, int> t2 = prepJavaString('b', s2);
		
		socket->send(get<0>(t2), get<1>(t2));
	}
	
	//3
	char c3[2];
	c3[0] = 'c';
	c3[1] = '\n';	
	socket->send(c3, 2);
}

//c++ to java util functions
tuple<char*, int> WMController::prepJavaString(char stage, string data)
{
	char charArray[256];
	int len = sprintf(charArray, "%c%s\n", stage, data.c_str());
	cout << "prep java: " << charArray << len << '\t' << strlen(charArray) << endl;
	if(len != data.length() + 2)
		cout << "sprintf error" << endl;
	/*cout << "last char " << (int)charArray[len - 1] << endl;
	cout << "2nd to last " << (int)charArray[len - 2] << endl;
	cout << "3rd to last " << (int)charArray[len - 3] << endl;
	cout << "4th " << (int)charArray[len - 4] << endl;*/
	return make_tuple<char*, int>(charArray, len);
}

//debug methods
void WMController::printBytes(char* c, int len)
{
	for(int i = 0; i < len; ++i)
	{
		cout << (int)c[i] << " ";
	}
	cout << endl;
}

bool WMController::hasData()
{
	return _packets.size() > 0;	
}

int WMController::getDataAsInt()
{
	//pop off packet
	InboundPacket* p;
	bool hadData = _packets.get(p);
	
	if(hadData)
	{
		int data = p->toPluginHandlerInt();
		if(p->getType() == "Cue Toggle" )
			_toggles.add(dynamic_cast<CueToggle*>(p));
		else if(p->getType() == "New Subject")
			_newSubjects.add(dynamic_cast<NewSubject*>(p));
		else
			delete p;
		return data;
	}
	else
		return 0;
}

tuple<string, bool> WMController::getToggle()
{
	CueToggle* p;
	bool hadData = _toggles.get(p);
	
	if(hadData)
	{
		tuple<string, bool> t = p->getAsTuple();
		delete p;
		return t;
	}
	else
		return make_tuple<string, bool>("", false);
}

string WMController::getNewSubject()
{
	NewSubject* p;
	bool hadData = _newSubjects.get(p);
	
	if(hadData)
	{
		string t = p->toString();
		delete p;
		return t;
	}
	else
		return "";
}



//DEPRECATED
/* TODO: add this to the queue instead instead of directly 
void WMController::takeAction(InboundPacket* packet)
{
	if(packet->getType() == "Command")
	{
		executeCommand(dynamic_cast<Command*>(packet));
	}
	else if(packet->getType() == "StateRequest")
	{
		sendState(dynamic_cast<StateRequest*>(packet));
	}
}*/
string WMController::processData(CVRSocket * socket)
{
	//read stage number
	int num[2];
    if(!socket->recv(num,sizeof(int) * 2))
	{
		return "ERROR";
	}
	//get the packet
	char buf[num[1]];
    if(!socket->recv(buf,num[1]))
    {
        return "ERROR";
	}
	buf[num[1] - 1] = (char)0;
	//cout << num[0] << " echo: " << buf << endl;
	
	return string(buf);
}

bool WMController::hasToggles()
{
	return _toggles.size() > 0;
}

bool WMController::hasNewSubjects()
{
	return _newSubjects.size() > 0;
}

};
