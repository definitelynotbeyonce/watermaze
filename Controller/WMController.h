/**
 * @file WMController.h
 */

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
#include <cvrConfig/ConfigManager.h>

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

#include "ControllerQueue.h"

#include "OutboundPackets/TrialSetup.h"
#include "OutboundPackets/OutboundPacket.h"
#include "OutboundPackets/StateUpdate.h"

#include "InboundPackets/InboundPacket.h"
#include "InboundPackets/Command.h"
#include "InboundPackets/StateRequest.h"
#include "InboundPackets/TrialSetupRequest.h"
#include "InboundPackets/CueListRequest.h"
#include "InboundPackets/CueToggle.h"
#include "InboundPackets/NewSubject.h"

using namespace std;
using namespace cvr;

namespace WaterMaze/** @todo this class will extend OpenThreads::Thread once this class becomes parallelized. : public OpenThreads::Thread*/
{
	class WaterMaze;
	/**
	 * @brief Handler for communication between CalVR and the android controller device.
	 */
	class WMController
	{
		public:
			/**
			 * Default Constructor
			 */
			WMController();
			/**
			 * @brief Constructor.
			 * @param wm Pointer to the WaterMaze this object is controlling.
			 */
			WMController(WaterMaze * wm);
			/**
			 * Destructor
			 */
			~WMController();
			
		// Inbound packet functions
			/**
			 * @brief Accept incoming connections
			 */
			void getConnections();
			/**
			 * @brief Check the sockets for incoming data
			 */
			void getData();
			
		// Outbound Packet Functions
			/**
			 * @param odp Packet to be sent.
			 * 
			 * Broadcast data to all connected controllers.  Parallelized using OpenMP threads.  
			 */
			void bCastOutboundPacket(OutboundPacket* obp);
			/**
			 * @param socket Pointer to the the CVRSocket belonging to a device.
			 * @param odp Packet to be sent.
			 * 
			 * Send data to a single controller.
			 */
			void singleDeviceOutput(CVRSocket* socket, OutboundPacket* obp);
			 
		// Data packet access methods
			/**
			 * Whether or not the android controller has sent data to CalVR.
			 */
			bool hasData();
			/**
			 * Return integer representation of data received from the android controller.
			 */
			int getDataAsInt();
			
			/**
			 * Whether or not the android controller has toggled a cue.
			 */
			bool hasToggles();
			/**
			 * Return toggle key and state of the cue toggled (as a tuple pair) by the android controller.
			 */ 
			tuple<string, bool> getToggle();
			
			/**
			 * Return unique identifier of the new test subject.
			 */
			bool hasNewSubjects();
			/**
			 * Whether or not the android controller has created a new test subject.
			 */
			string getNewSubject();
			
		//General Access Methods
			/**
			 * Returns the number of android devices connected.
			 */
			int getNumControllers();
		protected: 
		/* Methods */
			// Socket input functions
			bool processSocketInput(cvr::CVRSocket * socket);
			int processPacket(int stage, cvr::CVRSocket * socket, InboundPacket* &packet);
			InboundPacket* processType(string type, CVRSocket* socket);
			
			// Packet response functions
			void sendState(cvr::CVRSocket * destination);
			
			// C++ to Java util functions
			string prepJavaString(char stage, string data);	
						
			// Debugging Methods
			void printBytes(char* c, int len);
			
		/* Members */
			WaterMaze* _wm;	///< maze being controlled.  @todo Remove the need for this reference.  Data requests (state, cue list) can instead be processed inside WM and passed to the controller.
			
			// Connection Data
			int _portNo;
			string _myIP;
			string _controllerIP;
			
			// Inbound Socket Connection Handler
			MultiListenSocket* _incoming;	///< MultiListenSocket used for accepting connections from android devices
			
			// I/O socket
			vector<CVRSocket*> _socketList;	///< Sockets for connected android devices.
			
			// Packet queues used for threading
			ControllerQueue<InboundPacket*> _packets;
			ControllerQueue<CueToggle*> _toggles;
			ControllerQueue<NewSubject*> _newSubjects;
		
		/** @todo Parallelize socket input and turn this into an active listen thread with WaterMaze querying the _packets ControllerQueue */
		/*
		private:
			bool _mKill;
			virtual void run();
			OpenThreads::Mutex _mutex;
			
			ControllerQueue<InboundPacket*> _packets;
			void * _context;
			void * _subscriber;
		*/
	};
};


#endif
