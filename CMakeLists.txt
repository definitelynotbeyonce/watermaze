SET(LIB_NAME WaterMaze)
SET(PLUGIN_HEADERS
    WaterMaze.h

)

ADD_LIBRARY(${LIB_NAME}
    "SHARED"
    ${PLUGIN_HEADERS}
    WaterMaze.cpp
	WaterMazeTakeAction.cpp
	EyeData.cpp
	Paradigm.cpp
	Path.cpp
	PathData.cpp

	Controller/WMController.cpp
	Controller/ControllerQueue.cpp

	Controller/OutboundPackets/OutboundPacket.cpp
	Controller/OutboundPackets/TrialSetup.cpp
	Controller/OutboundPackets/StateUpdate.cpp
	Controller/OutboundPackets/CueList.cpp
	Controller/OutboundPackets/GeneralComm.cpp

	Controller/InboundPackets/InboundPacket.cpp
	Controller/InboundPackets/StateRequest.cpp
	Controller/InboundPackets/Command.cpp
	Controller/InboundPackets/TrialSetupRequest.cpp
	Controller/InboundPackets/CueListRequest.cpp
	Controller/InboundPackets/CueToggle.cpp
	Controller/InboundPackets/NewSubject.cpp

	Cue.cpp
	Cues/WallCue.cpp
	Cues/FloorCue.cpp
	Cues/EndCue.cpp
	Cues/StartCue.cpp
	Cues/AuditoryCue.cpp

	SyncedRandomNumber.cpp
)

INCLUDE_DIRECTORIES(${OSG_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(${LIB_NAME} ${OSG_LIBRARIES})

FIND_PACKAGE(OASCLIENT)
INCLUDE_DIRECTORIES(${OASCLIENT_DIR})
TARGET_LINK_LIBRARIES(${LIB_NAME} ${OASCLIENT_LIBRARY})

ADD_CALVR_LIBRARIES(${LIB_NAME})

INSTALL(TARGETS ${LIB_NAME} DESTINATION lib/plugins)

if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++0x -fopenmp -w")
endif()

OPTION(MAKE_WATERMAZE_DOCUMENTATION "Create Doxygen documentation" OFF)

IF(MAKE_WATERMAZE_DOCUMENTATION)
    FIND_PACKAGE(Doxygen REQUIRED)

    CONFIGURE_FILE(doc/Doxyfile.cmake doc/Doxyfile.txt)

    ADD_CUSTOM_TARGET(documentation ALL ${DOXYGEN_EXECUTABLE} doc/Doxyfile.txt)
ENDIF(MAKE_WATERMAZE_DOCUMENTATION)
