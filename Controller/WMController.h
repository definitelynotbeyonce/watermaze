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

//Parallelization
#include <omp.h>

//Project Headers
#include "../WaterMaze.h"
#include "TrialSetup.h"
#include "OutboundPacket.h"
#include "StateUpdate.h"


using namespace std;
using namespace cvr;

namespace WaterMaze
{
	class WaterMaze;
	class WMController
	{
		public:
			WMController();
			WMController(WaterMaze * wm);
			~WMController();
			
			//inbound packet functions
			void getConnections();
			void getData();
			
			//Outbound Packet Functions
			void bCastOutboundPacketResults(OutboundPacket* obp);
			void bCastOutboundPacketState(StateUpdate* su);	//these could be wrapped into one function but they broadcast over different channels
			
		protected: 
			//Process Input from sockets
			bool processSocketInput(cvr::CVRSocket * socket);
			
			//Input Handlers
			void categorizeConnection(string type, CVRSocket* s);
			void parseDataLine(string type, string dataLine);
			void commandHandler(string dataLine);	//TODO
			
			//Response Functions
			void responseHandler(string type, string dataLine, CVRSocket* socket);
			void stateRequestResponseHandler(CVRSocket* socket);
			
			//c++ to java util functions
			tuple<char*, int> prepJavaString(char stage, string data);	
						
			//Debugging Methods
			void printBytes(char* c, int len);
			
			//Members
			//MVC
			WaterMaze * _wm;
			
			//Connection Data
			int _portNo;
			string _myIP;
			string _controllerIP;
			
			//Inbound Sockets
			MultiListenSocket* _incoming;
			vector<CVRSocket*> _socketList;	
			
			//Outbound Sockets
			vector<CVRSocket*> _ALstateSockets;	
			vector<CVRSocket*> _ALdataSockets;	
	};
};


#endif
