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
	//InboundPacket* packet;
	string type = "";
	int result = processPacket(1, socket, type);
	if(result < 0)
		return false;
	
	string data = processData(socket);	
	if(data == "ERROR")
		return false;
	
	string last = processData(socket);
	if(data == "ERROR")
		return false;
		
	//process
	takeAction(type, data, socket);
	return true;
}

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
	cout << num[0] << " echo: " << buf << endl;
	
	return string(buf);
}

int WMController::processPacket(int stage, CVRSocket * socket, string &data)
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
	cout << num[0] << " echo: " << buf << endl;
	
	//process packet
	switch(stage)
	{
		case 1:
			if(num[0] == 1 && data == "")
			{
				//packet = processType(string(buf), socket);
				data += string(buf);
			}
			break;
		case 2:
			if(num[0] == 2)
			{
				data += string(buf);
			}
			break;
		default:
			break;
	}
	
	return num[0];	//return the stage as determined by the incoming packet used as an LCV	
}
/*
InboundPacket* WMController::processType(string type, CVRSocket* socket)
{
	if(type == "Command"){
		return (new Command());
	}
	else if(type == "State Request"){
		return (new StateRequest(socket));
	}
	else{
		//this shouldnt happen
		cout << "error unknown packet type: " << type << endl;
		return NULL;
	}
}*/

/*
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
void WMController::takeAction(string type, string data, CVRSocket* socket)
{
	if(type == "Command")
	{
		executeCommand(data);
	}
	else if(type == "State Request")
	{
		sendState(socket);
	}
}

void WMController::executeCommand(string command)
{
	if(command == "Load Geometry")
	{
		_wm->load();
	}
	else if(command == "Next Paradigm")
	{
		_wm->changeParadigm(1);
	}
	else if(command == "Previous Paradigm")
	{
		_wm->changeParadigm(-1);
	}
	else if(command == "Start")
	{
		_wm->startStop();
	}
	else if(command == "Stop")
	{
		_wm->startStop();
	}
	else if(command == "Play")
	{
		_wm->playPause();
	}
	else if(command == "Pause")
	{
		_wm->playPause();
	}
	else if(command == "Next Trial")
	{
		_wm->changeTrial(1);
	}
	else if(command == "Add Trial")
	{
		_wm->addTrial();
	}
	else {
		cout << "error unknown command: " << command << endl;
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
	int len = sprintf(charArray, "%c%s", stage, data.c_str());
	
	if(len != data.length() + 1)
		cout << "sprintf error" << endl;
		
	charArray[len] = '\n';	//java is expecting a newline not null terminator
	
	return make_tuple<char*, int>(charArray, len + 1);
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

};
