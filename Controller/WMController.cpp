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
void WMController::bCastOutboundPacketResults(OutboundPacket* obp)
{
	cout << "broadcasting outbound packet" << endl;
	int n = _ALdataSockets.size();	//LCV used for omp parallelization
	
	//1
	tuple<char*, int> t1 = prepJavaString('a', obp->getType());
	#pragma omp parallel for schedule(dynamic)
	for(int i = 0; i < n; ++i)
	{
		_ALdataSockets[i]->send(get<0>(t1), get<1>(t1));
	}
	
	//2
	string s2;
	while((s2 = obp->getLine()) != "NULL")
	{
		tuple<char*, int> t2 = prepJavaString('b', s2);
		#pragma omp parallel for schedule(dynamic)
		for(int i = 0; i < n; ++i)
		{
			_ALdataSockets[i]->send(get<0>(t2), get<1>(t2));
		}
	}
	
	//3
	char c3[2];
	c3[0] = 'c';
	c3[1] = '\n';	
	#pragma omp parallel for schedule(dynamic)
	for(int i = 0; i < n; ++i)
	{
		_ALdataSockets[i]->send(c3, 2);
	}
}

void WMController::bCastOutboundPacketState(StateUpdate* su)
{
	//1
	tuple<char*, int> t1 = prepJavaString('a', su->getType());
	#pragma omp parallel for schedule(dynamic)
	for(int i = 0; i < _ALdataSockets.size(); ++i)
	{
		_ALdataSockets[i]->send(get<0>(t1), get<1>(t1));
	}
	
	//2
	string s2;
	while((s2 = su->getLine()) != "NULL")
	{
		tuple<char*, int> t2 = prepJavaString('b', s2);
		#pragma omp parallel for schedule(dynamic)
		for(int i = 0; i < _ALdataSockets.size(); ++i)
		{
			_ALdataSockets[i]->send(get<0>(t2), get<1>(t2));
		}
	}
	
	//3
	char c3[2];
	c3[0] = 'c';
	c3[1] = '\n';	
	#pragma omp parallel for schedule(dynamic)
	for(int i = 0; i < _ALdataSockets.size(); ++i)
	{
		_ALdataSockets[i]->send(c3, 2);
	}
}

//process input
bool WMController::processSocketInput(cvr::CVRSocket * socket)
{
	//stage 1
	int num[2];
    if(!socket->recv(num,sizeof(int) * 2))
	{
		return false;
	}
	//get the packet
	char typeBuf[num[1]];
    if(!socket->recv(typeBuf,num[1]))
        return false;
    else
    {
		//turn last line feed into a null terminator. convert to string
		typeBuf[num[1] - 1] = (char)0;
		cout << "echo: " << typeBuf << endl;
	}
	
	//stage 2
	string dataLine;
	if(!socket->recv(num,sizeof(int) * 2))
	{
		return false;
	}
	//get the packet
	char dataBuf[num[1]];
	if(!socket->recv(dataBuf,num[1]))
		return false;
	else
	{
		//turn last line feed into a null terminator. convert to string
		dataBuf[num[1] - 1] = (char)0;
		cout << "echo: " << dataBuf << endl;
		parseDataLine(string(typeBuf), string(dataBuf));
	}
	
	//stage 3
	if(!socket->recv(num,sizeof(int) * 2))
	{
		return false;
	}
	//get the packet
	char lastBuf[num[1]];
	if(!socket->recv(lastBuf,num[1]))
		return false;
	else
	{
		//turn last line feed into a null terminator. convert to string
		lastBuf[num[1] - 1] = (char)0;
		cout << "echo: " << lastBuf << endl;
		responseHandler(string(typeBuf), string(dataBuf), socket);
	}
	
    return true;
}

//input handlers
void WMController::categorizeConnection(string type, CVRSocket* s)
{
	cout << "categorizeConnection" << endl;
	//place into proper list
	if(type == "control")
	{	// Control Socket
		cout << "this is a control socket" << endl;
	}else if(type == "active listen control")
	{	// Broadcast state changes
		cout << "this is an active listen control" << endl;
		_ALstateSockets.push_back(s);
	}else if(type == "active listen results")
	{	// Data Point results broadcasting
		cout << "this is an active listen results" << endl;
		_ALdataSockets.push_back(s);
	}
	else{	//error?
		cout << "This should not happen" << endl;
	}
}

void WMController::parseDataLine(string type, string dataLine)
{
	if(type == "Command"){
		commandHandler(dataLine);
	}else if(type == "State Request"){
		//do nothing yet.
	}else if(type == "Initial Connect"){
		//categorizeConnection(type, 
	}else{
		//this shouldnt happen
		cout << "error unknown packet type: " << type << endl;
	}
}

void WMController::commandHandler(string dataLine)
{
	if(dataLine == "Load Geometry")
	{
		_wm->load();
	}
	else if(dataLine == "Next Paradigm")
	{
		_wm->changeParadigm(1);
	}
	else if(dataLine == "Previous Paradigm")
	{
		_wm->changeParadigm(-1);
	}
	else if(dataLine == "Start")
	{
		_wm->startStop();
	}
	else if(dataLine == "Stop")
	{
		_wm->startStop();
	}
	else if(dataLine == "Play")
	{
		_wm->playPause();
	}
	else if(dataLine == "Pause")
	{
		_wm->playPause();
	}
	else if(dataLine == "Next Trial")
	{
		_wm->changeTrial(1);
	}
	else if(dataLine == "Add Trial")
	{
		_wm->addTrial();
	}
	else {
		cout << "error unknown command: " << dataLine << endl;
	}
}

//response functions
void WMController::responseHandler(string type, string dataLine, CVRSocket* socket)
{
	if(type == "Command"){
		StateUpdate* su = new StateUpdate(_wm->getState());
		bCastOutboundPacketState(su);
		delete su;
	} else if(type =="State Request"){
		stateRequestResponseHandler(socket);
	} else if(type == "Initial Connect"){
		categorizeConnection(dataLine, socket);
	}
	else{
		cout << "unknown type.  not responding." << endl;
	}
}

void WMController::stateRequestResponseHandler(CVRSocket* socket)
{
	//TODO: changed this to a state update
	//1
	tuple<char*, int> t1 = prepJavaString('a', "State Update");
	socket->send(get<0>(t1), get<1>(t1));
	
	//2
	tuple<char*, int> t2 = prepJavaString('a', _wm->getState());
	socket->send(get<0>(t2), get<1>(t2));
	
	//3
	char resp3[2];
	resp3[0] = 'c';
	resp3[1] = (char)10;	//java expects a newline	
	socket->send(resp3, 2);

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
