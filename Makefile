# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake28

# The command to remove a file.
RM = /usr/bin/cmake28 -E remove -f

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake28

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/srunyon/workStuff/CVRPlugins

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/srunyon/workStuff/CVRPlugins

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/usr/bin/ccmake28 -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target install
install: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Install the project..."
	/usr/bin/cmake28 -P cmake_install.cmake
.PHONY : install

# Special rule for the target install
install/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Install the project..."
	/usr/bin/cmake28 -P cmake_install.cmake
.PHONY : install/fast

# Special rule for the target install/local
install/local: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing only the local directory..."
	/usr/bin/cmake28 -DCMAKE_INSTALL_LOCAL_ONLY=1 -P cmake_install.cmake
.PHONY : install/local

# Special rule for the target install/local
install/local/fast: install/local
.PHONY : install/local/fast

# Special rule for the target install/strip
install/strip: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing the project stripped..."
	/usr/bin/cmake28 -DCMAKE_INSTALL_DO_STRIP=1 -P cmake_install.cmake
.PHONY : install/strip

# Special rule for the target install/strip
install/strip/fast: install/strip
.PHONY : install/strip/fast

# Special rule for the target list_install_components
list_install_components:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Available install components are: \"Unspecified\""
.PHONY : list_install_components

# Special rule for the target list_install_components
list_install_components/fast: list_install_components
.PHONY : list_install_components/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake28 -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	cd /home/srunyon/workStuff/CVRPlugins && $(CMAKE_COMMAND) -E cmake_progress_start /home/srunyon/workStuff/CVRPlugins/CMakeFiles /home/srunyon/workStuff/CVRPlugins/calit2/WaterMaze/CMakeFiles/progress.marks
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f CMakeFiles/Makefile2 calit2/WaterMaze/all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/srunyon/workStuff/CVRPlugins/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f CMakeFiles/Makefile2 calit2/WaterMaze/clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f CMakeFiles/Makefile2 calit2/WaterMaze/preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f CMakeFiles/Makefile2 calit2/WaterMaze/preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	cd /home/srunyon/workStuff/CVRPlugins && $(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

# Convenience name for target.
calit2/WaterMaze/CMakeFiles/WaterMaze.dir/rule:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f CMakeFiles/Makefile2 calit2/WaterMaze/CMakeFiles/WaterMaze.dir/rule
.PHONY : calit2/WaterMaze/CMakeFiles/WaterMaze.dir/rule

# Convenience name for target.
WaterMaze: calit2/WaterMaze/CMakeFiles/WaterMaze.dir/rule
.PHONY : WaterMaze

# fast build rule for target.
WaterMaze/fast:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build
.PHONY : WaterMaze/fast

Controller/ControllerQueue.o: Controller/ControllerQueue.cpp.o
.PHONY : Controller/ControllerQueue.o

# target to build an object file
Controller/ControllerQueue.cpp.o:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Controller/ControllerQueue.cpp.o
.PHONY : Controller/ControllerQueue.cpp.o

Controller/ControllerQueue.i: Controller/ControllerQueue.cpp.i
.PHONY : Controller/ControllerQueue.i

# target to preprocess a source file
Controller/ControllerQueue.cpp.i:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Controller/ControllerQueue.cpp.i
.PHONY : Controller/ControllerQueue.cpp.i

Controller/ControllerQueue.s: Controller/ControllerQueue.cpp.s
.PHONY : Controller/ControllerQueue.s

# target to generate assembly for a file
Controller/ControllerQueue.cpp.s:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Controller/ControllerQueue.cpp.s
.PHONY : Controller/ControllerQueue.cpp.s

Controller/InboundPackets/Command.o: Controller/InboundPackets/Command.cpp.o
.PHONY : Controller/InboundPackets/Command.o

# target to build an object file
Controller/InboundPackets/Command.cpp.o:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Controller/InboundPackets/Command.cpp.o
.PHONY : Controller/InboundPackets/Command.cpp.o

Controller/InboundPackets/Command.i: Controller/InboundPackets/Command.cpp.i
.PHONY : Controller/InboundPackets/Command.i

# target to preprocess a source file
Controller/InboundPackets/Command.cpp.i:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Controller/InboundPackets/Command.cpp.i
.PHONY : Controller/InboundPackets/Command.cpp.i

Controller/InboundPackets/Command.s: Controller/InboundPackets/Command.cpp.s
.PHONY : Controller/InboundPackets/Command.s

# target to generate assembly for a file
Controller/InboundPackets/Command.cpp.s:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Controller/InboundPackets/Command.cpp.s
.PHONY : Controller/InboundPackets/Command.cpp.s

Controller/InboundPackets/CueListRequest.o: Controller/InboundPackets/CueListRequest.cpp.o
.PHONY : Controller/InboundPackets/CueListRequest.o

# target to build an object file
Controller/InboundPackets/CueListRequest.cpp.o:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Controller/InboundPackets/CueListRequest.cpp.o
.PHONY : Controller/InboundPackets/CueListRequest.cpp.o

Controller/InboundPackets/CueListRequest.i: Controller/InboundPackets/CueListRequest.cpp.i
.PHONY : Controller/InboundPackets/CueListRequest.i

# target to preprocess a source file
Controller/InboundPackets/CueListRequest.cpp.i:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Controller/InboundPackets/CueListRequest.cpp.i
.PHONY : Controller/InboundPackets/CueListRequest.cpp.i

Controller/InboundPackets/CueListRequest.s: Controller/InboundPackets/CueListRequest.cpp.s
.PHONY : Controller/InboundPackets/CueListRequest.s

# target to generate assembly for a file
Controller/InboundPackets/CueListRequest.cpp.s:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Controller/InboundPackets/CueListRequest.cpp.s
.PHONY : Controller/InboundPackets/CueListRequest.cpp.s

Controller/InboundPackets/CueToggle.o: Controller/InboundPackets/CueToggle.cpp.o
.PHONY : Controller/InboundPackets/CueToggle.o

# target to build an object file
Controller/InboundPackets/CueToggle.cpp.o:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Controller/InboundPackets/CueToggle.cpp.o
.PHONY : Controller/InboundPackets/CueToggle.cpp.o

Controller/InboundPackets/CueToggle.i: Controller/InboundPackets/CueToggle.cpp.i
.PHONY : Controller/InboundPackets/CueToggle.i

# target to preprocess a source file
Controller/InboundPackets/CueToggle.cpp.i:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Controller/InboundPackets/CueToggle.cpp.i
.PHONY : Controller/InboundPackets/CueToggle.cpp.i

Controller/InboundPackets/CueToggle.s: Controller/InboundPackets/CueToggle.cpp.s
.PHONY : Controller/InboundPackets/CueToggle.s

# target to generate assembly for a file
Controller/InboundPackets/CueToggle.cpp.s:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Controller/InboundPackets/CueToggle.cpp.s
.PHONY : Controller/InboundPackets/CueToggle.cpp.s

Controller/InboundPackets/InboundPacket.o: Controller/InboundPackets/InboundPacket.cpp.o
.PHONY : Controller/InboundPackets/InboundPacket.o

# target to build an object file
Controller/InboundPackets/InboundPacket.cpp.o:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Controller/InboundPackets/InboundPacket.cpp.o
.PHONY : Controller/InboundPackets/InboundPacket.cpp.o

Controller/InboundPackets/InboundPacket.i: Controller/InboundPackets/InboundPacket.cpp.i
.PHONY : Controller/InboundPackets/InboundPacket.i

# target to preprocess a source file
Controller/InboundPackets/InboundPacket.cpp.i:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Controller/InboundPackets/InboundPacket.cpp.i
.PHONY : Controller/InboundPackets/InboundPacket.cpp.i

Controller/InboundPackets/InboundPacket.s: Controller/InboundPackets/InboundPacket.cpp.s
.PHONY : Controller/InboundPackets/InboundPacket.s

# target to generate assembly for a file
Controller/InboundPackets/InboundPacket.cpp.s:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Controller/InboundPackets/InboundPacket.cpp.s
.PHONY : Controller/InboundPackets/InboundPacket.cpp.s

Controller/InboundPackets/NewSubject.o: Controller/InboundPackets/NewSubject.cpp.o
.PHONY : Controller/InboundPackets/NewSubject.o

# target to build an object file
Controller/InboundPackets/NewSubject.cpp.o:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Controller/InboundPackets/NewSubject.cpp.o
.PHONY : Controller/InboundPackets/NewSubject.cpp.o

Controller/InboundPackets/NewSubject.i: Controller/InboundPackets/NewSubject.cpp.i
.PHONY : Controller/InboundPackets/NewSubject.i

# target to preprocess a source file
Controller/InboundPackets/NewSubject.cpp.i:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Controller/InboundPackets/NewSubject.cpp.i
.PHONY : Controller/InboundPackets/NewSubject.cpp.i

Controller/InboundPackets/NewSubject.s: Controller/InboundPackets/NewSubject.cpp.s
.PHONY : Controller/InboundPackets/NewSubject.s

# target to generate assembly for a file
Controller/InboundPackets/NewSubject.cpp.s:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Controller/InboundPackets/NewSubject.cpp.s
.PHONY : Controller/InboundPackets/NewSubject.cpp.s

Controller/InboundPackets/StateRequest.o: Controller/InboundPackets/StateRequest.cpp.o
.PHONY : Controller/InboundPackets/StateRequest.o

# target to build an object file
Controller/InboundPackets/StateRequest.cpp.o:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Controller/InboundPackets/StateRequest.cpp.o
.PHONY : Controller/InboundPackets/StateRequest.cpp.o

Controller/InboundPackets/StateRequest.i: Controller/InboundPackets/StateRequest.cpp.i
.PHONY : Controller/InboundPackets/StateRequest.i

# target to preprocess a source file
Controller/InboundPackets/StateRequest.cpp.i:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Controller/InboundPackets/StateRequest.cpp.i
.PHONY : Controller/InboundPackets/StateRequest.cpp.i

Controller/InboundPackets/StateRequest.s: Controller/InboundPackets/StateRequest.cpp.s
.PHONY : Controller/InboundPackets/StateRequest.s

# target to generate assembly for a file
Controller/InboundPackets/StateRequest.cpp.s:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Controller/InboundPackets/StateRequest.cpp.s
.PHONY : Controller/InboundPackets/StateRequest.cpp.s

Controller/InboundPackets/TrialSetupRequest.o: Controller/InboundPackets/TrialSetupRequest.cpp.o
.PHONY : Controller/InboundPackets/TrialSetupRequest.o

# target to build an object file
Controller/InboundPackets/TrialSetupRequest.cpp.o:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Controller/InboundPackets/TrialSetupRequest.cpp.o
.PHONY : Controller/InboundPackets/TrialSetupRequest.cpp.o

Controller/InboundPackets/TrialSetupRequest.i: Controller/InboundPackets/TrialSetupRequest.cpp.i
.PHONY : Controller/InboundPackets/TrialSetupRequest.i

# target to preprocess a source file
Controller/InboundPackets/TrialSetupRequest.cpp.i:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Controller/InboundPackets/TrialSetupRequest.cpp.i
.PHONY : Controller/InboundPackets/TrialSetupRequest.cpp.i

Controller/InboundPackets/TrialSetupRequest.s: Controller/InboundPackets/TrialSetupRequest.cpp.s
.PHONY : Controller/InboundPackets/TrialSetupRequest.s

# target to generate assembly for a file
Controller/InboundPackets/TrialSetupRequest.cpp.s:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Controller/InboundPackets/TrialSetupRequest.cpp.s
.PHONY : Controller/InboundPackets/TrialSetupRequest.cpp.s

Controller/OutboundPackets/CueList.o: Controller/OutboundPackets/CueList.cpp.o
.PHONY : Controller/OutboundPackets/CueList.o

# target to build an object file
Controller/OutboundPackets/CueList.cpp.o:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Controller/OutboundPackets/CueList.cpp.o
.PHONY : Controller/OutboundPackets/CueList.cpp.o

Controller/OutboundPackets/CueList.i: Controller/OutboundPackets/CueList.cpp.i
.PHONY : Controller/OutboundPackets/CueList.i

# target to preprocess a source file
Controller/OutboundPackets/CueList.cpp.i:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Controller/OutboundPackets/CueList.cpp.i
.PHONY : Controller/OutboundPackets/CueList.cpp.i

Controller/OutboundPackets/CueList.s: Controller/OutboundPackets/CueList.cpp.s
.PHONY : Controller/OutboundPackets/CueList.s

# target to generate assembly for a file
Controller/OutboundPackets/CueList.cpp.s:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Controller/OutboundPackets/CueList.cpp.s
.PHONY : Controller/OutboundPackets/CueList.cpp.s

Controller/OutboundPackets/GeneralComm.o: Controller/OutboundPackets/GeneralComm.cpp.o
.PHONY : Controller/OutboundPackets/GeneralComm.o

# target to build an object file
Controller/OutboundPackets/GeneralComm.cpp.o:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Controller/OutboundPackets/GeneralComm.cpp.o
.PHONY : Controller/OutboundPackets/GeneralComm.cpp.o

Controller/OutboundPackets/GeneralComm.i: Controller/OutboundPackets/GeneralComm.cpp.i
.PHONY : Controller/OutboundPackets/GeneralComm.i

# target to preprocess a source file
Controller/OutboundPackets/GeneralComm.cpp.i:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Controller/OutboundPackets/GeneralComm.cpp.i
.PHONY : Controller/OutboundPackets/GeneralComm.cpp.i

Controller/OutboundPackets/GeneralComm.s: Controller/OutboundPackets/GeneralComm.cpp.s
.PHONY : Controller/OutboundPackets/GeneralComm.s

# target to generate assembly for a file
Controller/OutboundPackets/GeneralComm.cpp.s:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Controller/OutboundPackets/GeneralComm.cpp.s
.PHONY : Controller/OutboundPackets/GeneralComm.cpp.s

Controller/OutboundPackets/OutboundPacket.o: Controller/OutboundPackets/OutboundPacket.cpp.o
.PHONY : Controller/OutboundPackets/OutboundPacket.o

# target to build an object file
Controller/OutboundPackets/OutboundPacket.cpp.o:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Controller/OutboundPackets/OutboundPacket.cpp.o
.PHONY : Controller/OutboundPackets/OutboundPacket.cpp.o

Controller/OutboundPackets/OutboundPacket.i: Controller/OutboundPackets/OutboundPacket.cpp.i
.PHONY : Controller/OutboundPackets/OutboundPacket.i

# target to preprocess a source file
Controller/OutboundPackets/OutboundPacket.cpp.i:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Controller/OutboundPackets/OutboundPacket.cpp.i
.PHONY : Controller/OutboundPackets/OutboundPacket.cpp.i

Controller/OutboundPackets/OutboundPacket.s: Controller/OutboundPackets/OutboundPacket.cpp.s
.PHONY : Controller/OutboundPackets/OutboundPacket.s

# target to generate assembly for a file
Controller/OutboundPackets/OutboundPacket.cpp.s:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Controller/OutboundPackets/OutboundPacket.cpp.s
.PHONY : Controller/OutboundPackets/OutboundPacket.cpp.s

Controller/OutboundPackets/StateUpdate.o: Controller/OutboundPackets/StateUpdate.cpp.o
.PHONY : Controller/OutboundPackets/StateUpdate.o

# target to build an object file
Controller/OutboundPackets/StateUpdate.cpp.o:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Controller/OutboundPackets/StateUpdate.cpp.o
.PHONY : Controller/OutboundPackets/StateUpdate.cpp.o

Controller/OutboundPackets/StateUpdate.i: Controller/OutboundPackets/StateUpdate.cpp.i
.PHONY : Controller/OutboundPackets/StateUpdate.i

# target to preprocess a source file
Controller/OutboundPackets/StateUpdate.cpp.i:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Controller/OutboundPackets/StateUpdate.cpp.i
.PHONY : Controller/OutboundPackets/StateUpdate.cpp.i

Controller/OutboundPackets/StateUpdate.s: Controller/OutboundPackets/StateUpdate.cpp.s
.PHONY : Controller/OutboundPackets/StateUpdate.s

# target to generate assembly for a file
Controller/OutboundPackets/StateUpdate.cpp.s:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Controller/OutboundPackets/StateUpdate.cpp.s
.PHONY : Controller/OutboundPackets/StateUpdate.cpp.s

Controller/OutboundPackets/TrialSetup.o: Controller/OutboundPackets/TrialSetup.cpp.o
.PHONY : Controller/OutboundPackets/TrialSetup.o

# target to build an object file
Controller/OutboundPackets/TrialSetup.cpp.o:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Controller/OutboundPackets/TrialSetup.cpp.o
.PHONY : Controller/OutboundPackets/TrialSetup.cpp.o

Controller/OutboundPackets/TrialSetup.i: Controller/OutboundPackets/TrialSetup.cpp.i
.PHONY : Controller/OutboundPackets/TrialSetup.i

# target to preprocess a source file
Controller/OutboundPackets/TrialSetup.cpp.i:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Controller/OutboundPackets/TrialSetup.cpp.i
.PHONY : Controller/OutboundPackets/TrialSetup.cpp.i

Controller/OutboundPackets/TrialSetup.s: Controller/OutboundPackets/TrialSetup.cpp.s
.PHONY : Controller/OutboundPackets/TrialSetup.s

# target to generate assembly for a file
Controller/OutboundPackets/TrialSetup.cpp.s:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Controller/OutboundPackets/TrialSetup.cpp.s
.PHONY : Controller/OutboundPackets/TrialSetup.cpp.s

Controller/WMController.o: Controller/WMController.cpp.o
.PHONY : Controller/WMController.o

# target to build an object file
Controller/WMController.cpp.o:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Controller/WMController.cpp.o
.PHONY : Controller/WMController.cpp.o

Controller/WMController.i: Controller/WMController.cpp.i
.PHONY : Controller/WMController.i

# target to preprocess a source file
Controller/WMController.cpp.i:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Controller/WMController.cpp.i
.PHONY : Controller/WMController.cpp.i

Controller/WMController.s: Controller/WMController.cpp.s
.PHONY : Controller/WMController.s

# target to generate assembly for a file
Controller/WMController.cpp.s:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Controller/WMController.cpp.s
.PHONY : Controller/WMController.cpp.s

Cue.o: Cue.cpp.o
.PHONY : Cue.o

# target to build an object file
Cue.cpp.o:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Cue.cpp.o
.PHONY : Cue.cpp.o

Cue.i: Cue.cpp.i
.PHONY : Cue.i

# target to preprocess a source file
Cue.cpp.i:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Cue.cpp.i
.PHONY : Cue.cpp.i

Cue.s: Cue.cpp.s
.PHONY : Cue.s

# target to generate assembly for a file
Cue.cpp.s:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Cue.cpp.s
.PHONY : Cue.cpp.s

Cues/EndCue.o: Cues/EndCue.cpp.o
.PHONY : Cues/EndCue.o

# target to build an object file
Cues/EndCue.cpp.o:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Cues/EndCue.cpp.o
.PHONY : Cues/EndCue.cpp.o

Cues/EndCue.i: Cues/EndCue.cpp.i
.PHONY : Cues/EndCue.i

# target to preprocess a source file
Cues/EndCue.cpp.i:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Cues/EndCue.cpp.i
.PHONY : Cues/EndCue.cpp.i

Cues/EndCue.s: Cues/EndCue.cpp.s
.PHONY : Cues/EndCue.s

# target to generate assembly for a file
Cues/EndCue.cpp.s:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Cues/EndCue.cpp.s
.PHONY : Cues/EndCue.cpp.s

Cues/FloorCue.o: Cues/FloorCue.cpp.o
.PHONY : Cues/FloorCue.o

# target to build an object file
Cues/FloorCue.cpp.o:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Cues/FloorCue.cpp.o
.PHONY : Cues/FloorCue.cpp.o

Cues/FloorCue.i: Cues/FloorCue.cpp.i
.PHONY : Cues/FloorCue.i

# target to preprocess a source file
Cues/FloorCue.cpp.i:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Cues/FloorCue.cpp.i
.PHONY : Cues/FloorCue.cpp.i

Cues/FloorCue.s: Cues/FloorCue.cpp.s
.PHONY : Cues/FloorCue.s

# target to generate assembly for a file
Cues/FloorCue.cpp.s:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Cues/FloorCue.cpp.s
.PHONY : Cues/FloorCue.cpp.s

Cues/WallCue.o: Cues/WallCue.cpp.o
.PHONY : Cues/WallCue.o

# target to build an object file
Cues/WallCue.cpp.o:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Cues/WallCue.cpp.o
.PHONY : Cues/WallCue.cpp.o

Cues/WallCue.i: Cues/WallCue.cpp.i
.PHONY : Cues/WallCue.i

# target to preprocess a source file
Cues/WallCue.cpp.i:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Cues/WallCue.cpp.i
.PHONY : Cues/WallCue.cpp.i

Cues/WallCue.s: Cues/WallCue.cpp.s
.PHONY : Cues/WallCue.s

# target to generate assembly for a file
Cues/WallCue.cpp.s:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Cues/WallCue.cpp.s
.PHONY : Cues/WallCue.cpp.s

Experiment.o: Experiment.cpp.o
.PHONY : Experiment.o

# target to build an object file
Experiment.cpp.o:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Experiment.cpp.o
.PHONY : Experiment.cpp.o

Experiment.i: Experiment.cpp.i
.PHONY : Experiment.i

# target to preprocess a source file
Experiment.cpp.i:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Experiment.cpp.i
.PHONY : Experiment.cpp.i

Experiment.s: Experiment.cpp.s
.PHONY : Experiment.s

# target to generate assembly for a file
Experiment.cpp.s:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Experiment.cpp.s
.PHONY : Experiment.cpp.s

EyeData.o: EyeData.cpp.o
.PHONY : EyeData.o

# target to build an object file
EyeData.cpp.o:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/EyeData.cpp.o
.PHONY : EyeData.cpp.o

EyeData.i: EyeData.cpp.i
.PHONY : EyeData.i

# target to preprocess a source file
EyeData.cpp.i:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/EyeData.cpp.i
.PHONY : EyeData.cpp.i

EyeData.s: EyeData.cpp.s
.PHONY : EyeData.s

# target to generate assembly for a file
EyeData.cpp.s:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/EyeData.cpp.s
.PHONY : EyeData.cpp.s

Paradigm.o: Paradigm.cpp.o
.PHONY : Paradigm.o

# target to build an object file
Paradigm.cpp.o:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Paradigm.cpp.o
.PHONY : Paradigm.cpp.o

Paradigm.i: Paradigm.cpp.i
.PHONY : Paradigm.i

# target to preprocess a source file
Paradigm.cpp.i:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Paradigm.cpp.i
.PHONY : Paradigm.cpp.i

Paradigm.s: Paradigm.cpp.s
.PHONY : Paradigm.s

# target to generate assembly for a file
Paradigm.cpp.s:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Paradigm.cpp.s
.PHONY : Paradigm.cpp.s

Path.o: Path.cpp.o
.PHONY : Path.o

# target to build an object file
Path.cpp.o:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Path.cpp.o
.PHONY : Path.cpp.o

Path.i: Path.cpp.i
.PHONY : Path.i

# target to preprocess a source file
Path.cpp.i:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Path.cpp.i
.PHONY : Path.cpp.i

Path.s: Path.cpp.s
.PHONY : Path.s

# target to generate assembly for a file
Path.cpp.s:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/Path.cpp.s
.PHONY : Path.cpp.s

PathData.o: PathData.cpp.o
.PHONY : PathData.o

# target to build an object file
PathData.cpp.o:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/PathData.cpp.o
.PHONY : PathData.cpp.o

PathData.i: PathData.cpp.i
.PHONY : PathData.i

# target to preprocess a source file
PathData.cpp.i:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/PathData.cpp.i
.PHONY : PathData.cpp.i

PathData.s: PathData.cpp.s
.PHONY : PathData.s

# target to generate assembly for a file
PathData.cpp.s:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/PathData.cpp.s
.PHONY : PathData.cpp.s

WaterMaze.o: WaterMaze.cpp.o
.PHONY : WaterMaze.o

# target to build an object file
WaterMaze.cpp.o:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/WaterMaze.cpp.o
.PHONY : WaterMaze.cpp.o

WaterMaze.i: WaterMaze.cpp.i
.PHONY : WaterMaze.i

# target to preprocess a source file
WaterMaze.cpp.i:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/WaterMaze.cpp.i
.PHONY : WaterMaze.cpp.i

WaterMaze.s: WaterMaze.cpp.s
.PHONY : WaterMaze.s

# target to generate assembly for a file
WaterMaze.cpp.s:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/WaterMaze.cpp.s
.PHONY : WaterMaze.cpp.s

WaterMazeTakeAction.o: WaterMazeTakeAction.cpp.o
.PHONY : WaterMazeTakeAction.o

# target to build an object file
WaterMazeTakeAction.cpp.o:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/WaterMazeTakeAction.cpp.o
.PHONY : WaterMazeTakeAction.cpp.o

WaterMazeTakeAction.i: WaterMazeTakeAction.cpp.i
.PHONY : WaterMazeTakeAction.i

# target to preprocess a source file
WaterMazeTakeAction.cpp.i:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/WaterMazeTakeAction.cpp.i
.PHONY : WaterMazeTakeAction.cpp.i

WaterMazeTakeAction.s: WaterMazeTakeAction.cpp.s
.PHONY : WaterMazeTakeAction.s

# target to generate assembly for a file
WaterMazeTakeAction.cpp.s:
	cd /home/srunyon/workStuff/CVRPlugins && $(MAKE) -f calit2/WaterMaze/CMakeFiles/WaterMaze.dir/build.make calit2/WaterMaze/CMakeFiles/WaterMaze.dir/WaterMazeTakeAction.cpp.s
.PHONY : WaterMazeTakeAction.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... WaterMaze"
	@echo "... edit_cache"
	@echo "... install"
	@echo "... install/local"
	@echo "... install/strip"
	@echo "... list_install_components"
	@echo "... rebuild_cache"
	@echo "... Controller/ControllerQueue.o"
	@echo "... Controller/ControllerQueue.i"
	@echo "... Controller/ControllerQueue.s"
	@echo "... Controller/InboundPackets/Command.o"
	@echo "... Controller/InboundPackets/Command.i"
	@echo "... Controller/InboundPackets/Command.s"
	@echo "... Controller/InboundPackets/CueListRequest.o"
	@echo "... Controller/InboundPackets/CueListRequest.i"
	@echo "... Controller/InboundPackets/CueListRequest.s"
	@echo "... Controller/InboundPackets/CueToggle.o"
	@echo "... Controller/InboundPackets/CueToggle.i"
	@echo "... Controller/InboundPackets/CueToggle.s"
	@echo "... Controller/InboundPackets/InboundPacket.o"
	@echo "... Controller/InboundPackets/InboundPacket.i"
	@echo "... Controller/InboundPackets/InboundPacket.s"
	@echo "... Controller/InboundPackets/NewSubject.o"
	@echo "... Controller/InboundPackets/NewSubject.i"
	@echo "... Controller/InboundPackets/NewSubject.s"
	@echo "... Controller/InboundPackets/StateRequest.o"
	@echo "... Controller/InboundPackets/StateRequest.i"
	@echo "... Controller/InboundPackets/StateRequest.s"
	@echo "... Controller/InboundPackets/TrialSetupRequest.o"
	@echo "... Controller/InboundPackets/TrialSetupRequest.i"
	@echo "... Controller/InboundPackets/TrialSetupRequest.s"
	@echo "... Controller/OutboundPackets/CueList.o"
	@echo "... Controller/OutboundPackets/CueList.i"
	@echo "... Controller/OutboundPackets/CueList.s"
	@echo "... Controller/OutboundPackets/GeneralComm.o"
	@echo "... Controller/OutboundPackets/GeneralComm.i"
	@echo "... Controller/OutboundPackets/GeneralComm.s"
	@echo "... Controller/OutboundPackets/OutboundPacket.o"
	@echo "... Controller/OutboundPackets/OutboundPacket.i"
	@echo "... Controller/OutboundPackets/OutboundPacket.s"
	@echo "... Controller/OutboundPackets/StateUpdate.o"
	@echo "... Controller/OutboundPackets/StateUpdate.i"
	@echo "... Controller/OutboundPackets/StateUpdate.s"
	@echo "... Controller/OutboundPackets/TrialSetup.o"
	@echo "... Controller/OutboundPackets/TrialSetup.i"
	@echo "... Controller/OutboundPackets/TrialSetup.s"
	@echo "... Controller/WMController.o"
	@echo "... Controller/WMController.i"
	@echo "... Controller/WMController.s"
	@echo "... Cue.o"
	@echo "... Cue.i"
	@echo "... Cue.s"
	@echo "... Cues/EndCue.o"
	@echo "... Cues/EndCue.i"
	@echo "... Cues/EndCue.s"
	@echo "... Cues/FloorCue.o"
	@echo "... Cues/FloorCue.i"
	@echo "... Cues/FloorCue.s"
	@echo "... Cues/WallCue.o"
	@echo "... Cues/WallCue.i"
	@echo "... Cues/WallCue.s"
	@echo "... Experiment.o"
	@echo "... Experiment.i"
	@echo "... Experiment.s"
	@echo "... EyeData.o"
	@echo "... EyeData.i"
	@echo "... EyeData.s"
	@echo "... Paradigm.o"
	@echo "... Paradigm.i"
	@echo "... Paradigm.s"
	@echo "... Path.o"
	@echo "... Path.i"
	@echo "... Path.s"
	@echo "... PathData.o"
	@echo "... PathData.i"
	@echo "... PathData.s"
	@echo "... WaterMaze.o"
	@echo "... WaterMaze.i"
	@echo "... WaterMaze.s"
	@echo "... WaterMazeTakeAction.o"
	@echo "... WaterMazeTakeAction.i"
	@echo "... WaterMazeTakeAction.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	cd /home/srunyon/workStuff/CVRPlugins && $(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

