#include "WMController.h"

using namespace std;
using namespace cvr;

namespace WaterMaze
{
/* * * * * * * * * * * * * * * * * * * * * * * * 
 * 		Constructor / Destructor				
 * * * * * * * * * * * * * * * * * * * * * * * */
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

/* * * * * * * * * * * * * * * * * * * * * * * * 
 *		Inbound packet functions
 * * * * * * * * * * * * * * * * * * * * * * * */
void WMController::getConnections()
{
	CVRSocket* s = _incoming->accept();
	if(s != NULL)
	{
		if(_wm->getState() == "disconnected")
			_wm->changeState("connected");
		cout << "New socket" << endl;
		s->setNoDelay(true);
		_socketList.push_back(s);
	}
}

void WMController::getData()	
{
	//NOTE: code taken from FuturePatient Plugin.
	
	//Are there any packets?
	if(!_socketList.size())
    {
        return;
    }

    int maxfd = 0;

    fd_set socketsetR;
    FD_ZERO(&socketsetR);

	//Prep sockets for input.
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

	// Check every socket for data.  NOTE: not parallelizing this code ensures in order arrival
    for(std::vector<CVRSocket*>::iterator it = _socketList.begin(); it != _socketList.end(); )
    {
        if(FD_ISSET((*it)->getSocketFD(),&socketsetR))
        {
            if(!processSocketInput(*it))
            {
				// Controller was disconnected.
                cout << "Removing Control socket." << std::endl;
                delete *it;
                it = _socketList.erase(it);
            }
            else
            {
                ++it;
            }
        }
        else
        {
			// Socket had no data.
            ++it;
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * 
 * 		Outbound packet functions
 * * * * * * * * * * * * * * * * * * * * * * * */
void WMController::bCastOutboundPacket(OutboundPacket* obp)
{
	int n = _socketList.size();	//LCV used for omp parallelization
	
	// Stage 1
	string t1 = prepJavaString('a', obp->getType());
	#pragma omp parallel for schedule(dynamic)
	for(int i = 0; i < n; ++i)
	{
		char* c = (char*)t1.c_str();
		_socketList[i]->send(c, t1.length());
	}
	
	// Stage 2
	string s2;
	while((s2 = obp->getLine()) != "NULL")
	{
		string t2 = prepJavaString('b', s2);
		#pragma omp parallel for schedule(dynamic)
		for(int i = 0; i < n; ++i)
		{
			_socketList[i]->send((char*)t2.c_str(), t2.length());
		}
	}
	
	// Stage 3
	string t3 = prepJavaString('c', "");
	#pragma omp parallel for schedule(dynamic)
	for(int i = 0; i < n; ++i)
	{
		_socketList[i]->send((char*)t3.c_str(), t3.length());
	}
}

void WMController::singleDeviceOutput(CVRSocket* socket, OutboundPacket* obp)
{	
	// Stage 1
	string t1 = prepJavaString('a', obp->getType());
	char* c = (char*)t1.c_str();
	socket->send((char*)t1.c_str(), t1.length());
	
	// Stage 2
	string s2;
	while((s2 = obp->getLine()) != "NULL")
	{
		string t2 = prepJavaString('b', s2);
		char* c2 = (char*)t2.c_str();
		socket->send((char*)t2.c_str(), t2.length());
	}
	
	// Stage 3
	string t3 = prepJavaString('c', "");
	char* c3 = (char*)t3.c_str();
	socket->send((char*)t3.c_str(), t3.length());
}

/* * * * * * * * * * * * * * * * * * * * * * * * 
 * 		Data packet access methods
 * * * * * * * * * * * * * * * * * * * * * * * */
	//Data
bool WMController::hasData()
{
	return _packets.size() > 0;	
}

int WMController::getDataAsInt()
{
	// Pop off oldest packet
	InboundPacket* p;
	bool hadData = _packets.get(p);
	
	if(hadData)
	{
		// Get data as integer.  Will be returned later (after memory from packet is reclaimed).
		int data = p->toPluginHandlerInt();
		
		// Place packet into appropriate queue if it requires further processing.
		if(p->getType() == "Cue Toggle" )
			_toggles.add(dynamic_cast<CueToggle*>(p));
		else if(p->getType() == "New Subject")
			_newSubjects.add(dynamic_cast<NewSubject*>(p));
		else
			delete p;
		
		// Return data as integer.	
		return data;
	}
	else
	{
		return 0;	// Return value of 0 indicates no data.
	}
}

	//Toggles
bool WMController::hasToggles()
{
	return _toggles.size() > 0;
}

tuple<string, bool> WMController::getToggle()
{
	// Get oldest CueToggle from the packet queue.
	CueToggle* p;
	bool hadData = _toggles.get(p);
	
	// Pass this CueToggle to WaterMaze.
	if(hadData)
	{
		tuple<string, bool> toggle = p->getAsTuple();
		delete p;
		return toggle;
	}
	else
	{
		return make_tuple<string, bool>("", false);	// No cue was toggled.
	}
}

	//New Subject
bool WMController::hasNewSubjects()
{
	return _newSubjects.size() > 0;
}

string WMController::getNewSubject()
{
	// Pop off NewSubject packet from the queue.
	NewSubject* p;
	bool hadData = _newSubjects.get(p);
	
	// Was there a NewSubject packet? 
	if(hadData)
	{
		// Get the subjectID from the packet and return that value to WaterMaze.
		string subjectID = p->toString();
		delete p;
		return subjectID;
	}
	else
	{
		return "";
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * 
 * 		General Access Methods
 * * * * * * * * * * * * * * * * * * * * * * * */
int WMController::getNumControllers()
{
	return _socketList.size();
}

/* * * * * * * * * * * * * * * * * * * * * * * * 
 * 		Socket input functions
 * * * * * * * * * * * * * * * * * * * * * * * */
bool WMController::processSocketInput(cvr::CVRSocket * socket)
{
	//take in packet type byte
	InboundPacket* packet = NULL;
	int type = processPacket(1, socket, packet);
	if(type < 0)
		return false;
	
	//take in all stage 2 packets.
	int twoCount = 0;
	do
	{
		type = processPacket(2,socket, packet);
		if(type < 0)
		{
			return false;
		}
		twoCount += (type == 2)?1:0;
	}while(type == 2);
	
	//check to make sure packets were not received out of order.
	if(twoCount < 1)
	{
		cerr << "Packets out of order.  Stage 3 Packet recieved before a Stage 2 Packet." << endl;
	}
	
	//stage 3 packet has already been read in do-while loop.
	
	//respond
	if(packet->getType() == "State Request")
		sendState(socket);
	else if(packet->getType() == "Trial Setup Request")
		singleDeviceOutput(socket, _wm->getTrialSetup());
	else if(packet->getType() == "Cue List Request")
		singleDeviceOutput(socket, _wm->getCueList());
		
	//push the packet into the queue
	_packets.add(packet);
	
	//indicate that everything was read just fine.
	return true;
}



int WMController::processPacket(int stage, CVRSocket * socket, InboundPacket* &p)
{
	//read stage number
	int num[2];
	int javaIntSize = ConfigManager::getInt("Plugin.WaterMaze.AndroidController.JavaIntSize", 4);
    if(!socket->recv(num, javaIntSize * 2))	//take in stage integer and packet size.
	{
		return -1;
	}
	
	//get the packet
	char buf[num[1]];
    if(!socket->recv(buf,num[1]))
    {
        return -1;
	}
    
	// Turn last LF CR (how the Java string is terminated) into a null terminator. Convert to string.
	buf[num[1] - 1] = '\0';
	string dataLine = string(buf);
	
	//process packet
	switch(stage)
	{
		case 1:
			p = processType(dataLine, socket);
			break;
		case 2:
			if(num[0] == 2)
			{
				p->addLine(dataLine);
			}
			break;
		default:
			break;
	}
	
	return num[0];	//return the stage as determined by the incoming packet used as an LCV inside processSocketInput
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
	else
	{
		//This packet type has not been coded for yet.
		cerr << "error unknown packet type: " << type << endl;
		return NULL;
	}
}



/* * * * * * * * * * * * * * * * * * * * * * * * 
 * 		Packet response functions
 * * * * * * * * * * * * * * * * * * * * * * * */
void WMController::sendState(CVRSocket* destination)
{
	StateUpdate* su = new StateUpdate(_wm->getState());
	singleDeviceOutput(destination, su);
	delete su;
}

/* * * * * * * * * * * * * * * * * * * * * * * * 
 * 		C++ to Java util functions
 * * * * * * * * * * * * * * * * * * * * * * * */
string WMController::prepJavaString(char stage, string data)
{
	data.insert(0, 1, stage).append("\n");
	return data;
}

/* * * * * * * * * * * * * * * * * * * * * * * * 
 * 		Debug functions
 * * * * * * * * * * * * * * * * * * * * * * * */
void WMController::printBytes(char* c, int len)
{
	for(int i = 0; i < len; ++i)
	{
		cout << (int)c[i] << " ";
	}
	cout << endl;
}

};
