# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/renxl/share/catkin_ws/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/renxl/share/catkin_ws/build

# Include any dependencies generated for this target.
include ihome/aiui/CMakeFiles/say.dir/depend.make

# Include the progress variables for this target.
include ihome/aiui/CMakeFiles/say.dir/progress.make

# Include the compile flags for this target's objects.
include ihome/aiui/CMakeFiles/say.dir/flags.make

ihome/aiui/CMakeFiles/say.dir/src/say.cpp.o: ihome/aiui/CMakeFiles/say.dir/flags.make
ihome/aiui/CMakeFiles/say.dir/src/say.cpp.o: /home/renxl/share/catkin_ws/src/ihome/aiui/src/say.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/renxl/share/catkin_ws/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object ihome/aiui/CMakeFiles/say.dir/src/say.cpp.o"
	cd /home/renxl/share/catkin_ws/build/ihome/aiui && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/say.dir/src/say.cpp.o -c /home/renxl/share/catkin_ws/src/ihome/aiui/src/say.cpp

ihome/aiui/CMakeFiles/say.dir/src/say.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/say.dir/src/say.cpp.i"
	cd /home/renxl/share/catkin_ws/build/ihome/aiui && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/renxl/share/catkin_ws/src/ihome/aiui/src/say.cpp > CMakeFiles/say.dir/src/say.cpp.i

ihome/aiui/CMakeFiles/say.dir/src/say.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/say.dir/src/say.cpp.s"
	cd /home/renxl/share/catkin_ws/build/ihome/aiui && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/renxl/share/catkin_ws/src/ihome/aiui/src/say.cpp -o CMakeFiles/say.dir/src/say.cpp.s

ihome/aiui/CMakeFiles/say.dir/src/say.cpp.o.requires:

.PHONY : ihome/aiui/CMakeFiles/say.dir/src/say.cpp.o.requires

ihome/aiui/CMakeFiles/say.dir/src/say.cpp.o.provides: ihome/aiui/CMakeFiles/say.dir/src/say.cpp.o.requires
	$(MAKE) -f ihome/aiui/CMakeFiles/say.dir/build.make ihome/aiui/CMakeFiles/say.dir/src/say.cpp.o.provides.build
.PHONY : ihome/aiui/CMakeFiles/say.dir/src/say.cpp.o.provides

ihome/aiui/CMakeFiles/say.dir/src/say.cpp.o.provides.build: ihome/aiui/CMakeFiles/say.dir/src/say.cpp.o


# Object files for target say
say_OBJECTS = \
"CMakeFiles/say.dir/src/say.cpp.o"

# External object files for target say
say_EXTERNAL_OBJECTS =

/home/renxl/share/catkin_ws/devel/lib/aiui/say: ihome/aiui/CMakeFiles/say.dir/src/say.cpp.o
/home/renxl/share/catkin_ws/devel/lib/aiui/say: ihome/aiui/CMakeFiles/say.dir/build.make
/home/renxl/share/catkin_ws/devel/lib/aiui/say: /opt/ros/kinetic/lib/libroscpp.so
/home/renxl/share/catkin_ws/devel/lib/aiui/say: /usr/lib/x86_64-linux-gnu/libboost_signals.so
/home/renxl/share/catkin_ws/devel/lib/aiui/say: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so
/home/renxl/share/catkin_ws/devel/lib/aiui/say: /opt/ros/kinetic/lib/librosconsole.so
/home/renxl/share/catkin_ws/devel/lib/aiui/say: /opt/ros/kinetic/lib/librosconsole_log4cxx.so
/home/renxl/share/catkin_ws/devel/lib/aiui/say: /opt/ros/kinetic/lib/librosconsole_backend_interface.so
/home/renxl/share/catkin_ws/devel/lib/aiui/say: /usr/lib/x86_64-linux-gnu/liblog4cxx.so
/home/renxl/share/catkin_ws/devel/lib/aiui/say: /usr/lib/x86_64-linux-gnu/libboost_regex.so
/home/renxl/share/catkin_ws/devel/lib/aiui/say: /opt/ros/kinetic/lib/libxmlrpcpp.so
/home/renxl/share/catkin_ws/devel/lib/aiui/say: /opt/ros/kinetic/lib/libroscpp_serialization.so
/home/renxl/share/catkin_ws/devel/lib/aiui/say: /opt/ros/kinetic/lib/librostime.so
/home/renxl/share/catkin_ws/devel/lib/aiui/say: /opt/ros/kinetic/lib/libcpp_common.so
/home/renxl/share/catkin_ws/devel/lib/aiui/say: /usr/lib/x86_64-linux-gnu/libboost_system.so
/home/renxl/share/catkin_ws/devel/lib/aiui/say: /usr/lib/x86_64-linux-gnu/libboost_thread.so
/home/renxl/share/catkin_ws/devel/lib/aiui/say: /usr/lib/x86_64-linux-gnu/libboost_chrono.so
/home/renxl/share/catkin_ws/devel/lib/aiui/say: /usr/lib/x86_64-linux-gnu/libboost_date_time.so
/home/renxl/share/catkin_ws/devel/lib/aiui/say: /usr/lib/x86_64-linux-gnu/libboost_atomic.so
/home/renxl/share/catkin_ws/devel/lib/aiui/say: /usr/lib/x86_64-linux-gnu/libpthread.so
/home/renxl/share/catkin_ws/devel/lib/aiui/say: /usr/lib/x86_64-linux-gnu/libconsole_bridge.so
/home/renxl/share/catkin_ws/devel/lib/aiui/say: ihome/aiui/CMakeFiles/say.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/renxl/share/catkin_ws/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable /home/renxl/share/catkin_ws/devel/lib/aiui/say"
	cd /home/renxl/share/catkin_ws/build/ihome/aiui && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/say.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
ihome/aiui/CMakeFiles/say.dir/build: /home/renxl/share/catkin_ws/devel/lib/aiui/say

.PHONY : ihome/aiui/CMakeFiles/say.dir/build

ihome/aiui/CMakeFiles/say.dir/requires: ihome/aiui/CMakeFiles/say.dir/src/say.cpp.o.requires

.PHONY : ihome/aiui/CMakeFiles/say.dir/requires

ihome/aiui/CMakeFiles/say.dir/clean:
	cd /home/renxl/share/catkin_ws/build/ihome/aiui && $(CMAKE_COMMAND) -P CMakeFiles/say.dir/cmake_clean.cmake
.PHONY : ihome/aiui/CMakeFiles/say.dir/clean

ihome/aiui/CMakeFiles/say.dir/depend:
	cd /home/renxl/share/catkin_ws/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/renxl/share/catkin_ws/src /home/renxl/share/catkin_ws/src/ihome/aiui /home/renxl/share/catkin_ws/build /home/renxl/share/catkin_ws/build/ihome/aiui /home/renxl/share/catkin_ws/build/ihome/aiui/CMakeFiles/say.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : ihome/aiui/CMakeFiles/say.dir/depend
