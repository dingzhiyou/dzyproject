# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Produce verbose output by default.
VERBOSE = 1

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
CMAKE_SOURCE_DIR = /home/dada/project

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dada/project

# Include any dependencies generated for this target.
include CMakeFiles/test_shared_ptr.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/test_shared_ptr.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_shared_ptr.dir/flags.make

CMakeFiles/test_shared_ptr.dir/tests/test_shared_ptr.cc.o: CMakeFiles/test_shared_ptr.dir/flags.make
CMakeFiles/test_shared_ptr.dir/tests/test_shared_ptr.cc.o: tests/test_shared_ptr.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dada/project/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test_shared_ptr.dir/tests/test_shared_ptr.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_shared_ptr.dir/tests/test_shared_ptr.cc.o -c /home/dada/project/tests/test_shared_ptr.cc

CMakeFiles/test_shared_ptr.dir/tests/test_shared_ptr.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_shared_ptr.dir/tests/test_shared_ptr.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dada/project/tests/test_shared_ptr.cc > CMakeFiles/test_shared_ptr.dir/tests/test_shared_ptr.cc.i

CMakeFiles/test_shared_ptr.dir/tests/test_shared_ptr.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_shared_ptr.dir/tests/test_shared_ptr.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dada/project/tests/test_shared_ptr.cc -o CMakeFiles/test_shared_ptr.dir/tests/test_shared_ptr.cc.s

# Object files for target test_shared_ptr
test_shared_ptr_OBJECTS = \
"CMakeFiles/test_shared_ptr.dir/tests/test_shared_ptr.cc.o"

# External object files for target test_shared_ptr
test_shared_ptr_EXTERNAL_OBJECTS =

bin/test_shared_ptr: CMakeFiles/test_shared_ptr.dir/tests/test_shared_ptr.cc.o
bin/test_shared_ptr: CMakeFiles/test_shared_ptr.dir/build.make
bin/test_shared_ptr: lib/libdzy.so
bin/test_shared_ptr: CMakeFiles/test_shared_ptr.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/dada/project/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable bin/test_shared_ptr"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_shared_ptr.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_shared_ptr.dir/build: bin/test_shared_ptr

.PHONY : CMakeFiles/test_shared_ptr.dir/build

CMakeFiles/test_shared_ptr.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_shared_ptr.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_shared_ptr.dir/clean

CMakeFiles/test_shared_ptr.dir/depend:
	cd /home/dada/project && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dada/project /home/dada/project /home/dada/project /home/dada/project /home/dada/project/CMakeFiles/test_shared_ptr.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test_shared_ptr.dir/depend

