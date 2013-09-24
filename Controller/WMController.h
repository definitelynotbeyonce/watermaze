#ifndef _WMCONTROLLER_H_
#define _WMCONTROLLER_H_

//OSG Headers
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Material>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/ShapeDrawable>
#include <osg/Switch>
#include <osg/Texture2D>
#include <osgText/Text>
#include <osgDB/ReadFile>

//CalVR Headers
#include <cvrKernel/PluginHelper.h>
#include <cvrUtil/CVRSocket.h>
#include <cvrUtil/MultiListenSocket.h>

//Client/Server Headers TODO: do I need these
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <X11/Xlib.h>

//C++ std Headers
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <tuple>
#include <string.h>

//Thread Headers
#include <OpenThreads/Thread>
#include <OpenThreads/Mutex>

//Parallelization
#include <omp.h>

//Project Headers
#include "../WaterMaze.h"
#include "TrialSetup.h"
#include "OutboundPacket.h"
#include "StateUpdate.h"

#include "InboundPacket.h"
#include "Command.h"
#include "StateRequest.h"
#include "TrialSetupRequest.h"

#include "ControllerQueue.h"


using namespace std;
using namespace cvr;

namespace WaterMaze/*: public OpenThreads::Thread*/
{
	class WaterMaze;
	class WMController
	{
		public:
			WMController();
			WMController(WaterMaze * wm);
			~WMController();
			
			// inbound packet functions
			void getConnections();
			void getData();
			
			// Outbound Packet Functions
			void bCastOutboundPacket(OutboundPacket* obp);
			void singleDeviceOutput(CVRSocket* socket, OutboundPacket* obp);
			bool hasData();
			int getDataAsInt();
			
		protected: 
				//Methods
			// Process Input from sockets
			bool processSocketInput(cvr::CVRSocket * socket);
			int processPacket(int stage, cvr::CVRSocket * socket, InboundPacket* &packet);
			string processData(cvr::CVRSocket * socket);
			InboundPacket* processType(string type, CVRSocket* socket);
			
			// Response Functions
			void responseHandler(string type, string dataLine, CVRSocket* socket);
			void takeAction(string type, string data, CVRSocket* socket);
			void executeCommand(string command);
			void sendState(cvr::CVRSocket * destination);
			
			// c++ to java util functions
			tuple<char*, int> prepJavaString(char stage, string data);	
						
			// Debugging Methods
			void printBytes(char* c, int len);
			
				// Members
			WaterMaze* _wm;	//TODO: remove the need for this reference.
			
			// Connection Data
			int _portNo;
			string _myIP;
			string _controllerIP;
			
			// Inbound Socket Connection Handler
			MultiListenSocket* _incoming;
			
			// I/O socket
			vector<CVRSocket*> _socketList;	
			ControllerQueue<InboundPacket*> _packets;
			
		/*private:
			bool _mKill;
			virtual void run();
			OpenThreads::Mutex _mutex;
			
			ControllerQueue<InboundPacket*> _packets;
			void * _context;
			void * _subscriber;*/
	};
};


#endif
