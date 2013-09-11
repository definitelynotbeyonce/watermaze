SET(LIB_NAME WaterMaze)
SET(PLUGIN_HEADERS
    WaterMaze.h
	Cue.h
	Experiment.h
	EyeData.h
	Paradigm.h
	Path.h
	PathData.h
	Controller/WMController.h
	Controller/OutboundPacket.h
	Controller/TrialSetup.h
	Controller/StateUpdate.h
)

ADD_LIBRARY(${LIB_NAME}
    "SHARED"
    ${PLUGIN_HEADERS}
    WaterMaze.cpp
	Cue.cpp
	Experiment.cpp
	EyeData.cpp
	Paradigm.cpp
	Path.cpp
	PathData.cpp
	Controller/WMController.cpp
	Controller/OutboundPacket.cpp
	Controller/TrialSetup.cpp
	Controller/StateUpdate.cpp
)

FIND_PACKAGE(Collider)

INCLUDE_DIRECTORIES(${COLLIDER_DIR})
TARGET_LINK_LIBRARIES(${LIB_NAME} ${COLLIDER_LIBRARY})

INCLUDE_DIRECTORIES(${OSG_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(${LIB_NAME} ${OSG_LIBRARIES})

ADD_CALVR_LIBRARIES(${LIB_NAME})

INSTALL(TARGETS ${LIB_NAME} DESTINATION lib/plugins)

if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++0x")
endif()
