# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_COMMAND = /home/berkay/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/181.5281.33/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/berkay/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/181.5281.33/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/berkay/MEGAsync/TUM/Augmented Reality/exercise2"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/berkay/MEGAsync/TUM/Augmented Reality/exercise2/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/exercise2.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/exercise2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/exercise2.dir/flags.make

CMakeFiles/exercise2.dir/main.cpp.o: CMakeFiles/exercise2.dir/flags.make
CMakeFiles/exercise2.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/berkay/MEGAsync/TUM/Augmented Reality/exercise2/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/exercise2.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/exercise2.dir/main.cpp.o -c "/home/berkay/MEGAsync/TUM/Augmented Reality/exercise2/main.cpp"

CMakeFiles/exercise2.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/exercise2.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/berkay/MEGAsync/TUM/Augmented Reality/exercise2/main.cpp" > CMakeFiles/exercise2.dir/main.cpp.i

CMakeFiles/exercise2.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/exercise2.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/berkay/MEGAsync/TUM/Augmented Reality/exercise2/main.cpp" -o CMakeFiles/exercise2.dir/main.cpp.s

CMakeFiles/exercise2.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/exercise2.dir/main.cpp.o.requires

CMakeFiles/exercise2.dir/main.cpp.o.provides: CMakeFiles/exercise2.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/exercise2.dir/build.make CMakeFiles/exercise2.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/exercise2.dir/main.cpp.o.provides

CMakeFiles/exercise2.dir/main.cpp.o.provides.build: CMakeFiles/exercise2.dir/main.cpp.o


# Object files for target exercise2
exercise2_OBJECTS = \
"CMakeFiles/exercise2.dir/main.cpp.o"

# External object files for target exercise2
exercise2_EXTERNAL_OBJECTS =

exercise2: CMakeFiles/exercise2.dir/main.cpp.o
exercise2: CMakeFiles/exercise2.dir/build.make
exercise2: /usr/local/lib/libopencv_dnn.so.3.4.1
exercise2: /usr/local/lib/libopencv_ml.so.3.4.1
exercise2: /usr/local/lib/libopencv_objdetect.so.3.4.1
exercise2: /usr/local/lib/libopencv_shape.so.3.4.1
exercise2: /usr/local/lib/libopencv_stitching.so.3.4.1
exercise2: /usr/local/lib/libopencv_superres.so.3.4.1
exercise2: /usr/local/lib/libopencv_videostab.so.3.4.1
exercise2: /usr/local/lib/libopencv_calib3d.so.3.4.1
exercise2: /usr/local/lib/libopencv_features2d.so.3.4.1
exercise2: /usr/local/lib/libopencv_flann.so.3.4.1
exercise2: /usr/local/lib/libopencv_highgui.so.3.4.1
exercise2: /usr/local/lib/libopencv_photo.so.3.4.1
exercise2: /usr/local/lib/libopencv_video.so.3.4.1
exercise2: /usr/local/lib/libopencv_videoio.so.3.4.1
exercise2: /usr/local/lib/libopencv_imgcodecs.so.3.4.1
exercise2: /usr/local/lib/libopencv_imgproc.so.3.4.1
exercise2: /usr/local/lib/libopencv_core.so.3.4.1
exercise2: CMakeFiles/exercise2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/berkay/MEGAsync/TUM/Augmented Reality/exercise2/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable exercise2"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/exercise2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/exercise2.dir/build: exercise2

.PHONY : CMakeFiles/exercise2.dir/build

CMakeFiles/exercise2.dir/requires: CMakeFiles/exercise2.dir/main.cpp.o.requires

.PHONY : CMakeFiles/exercise2.dir/requires

CMakeFiles/exercise2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/exercise2.dir/cmake_clean.cmake
.PHONY : CMakeFiles/exercise2.dir/clean

CMakeFiles/exercise2.dir/depend:
	cd "/home/berkay/MEGAsync/TUM/Augmented Reality/exercise2/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/berkay/MEGAsync/TUM/Augmented Reality/exercise2" "/home/berkay/MEGAsync/TUM/Augmented Reality/exercise2" "/home/berkay/MEGAsync/TUM/Augmented Reality/exercise2/cmake-build-debug" "/home/berkay/MEGAsync/TUM/Augmented Reality/exercise2/cmake-build-debug" "/home/berkay/MEGAsync/TUM/Augmented Reality/exercise2/cmake-build-debug/CMakeFiles/exercise2.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/exercise2.dir/depend

