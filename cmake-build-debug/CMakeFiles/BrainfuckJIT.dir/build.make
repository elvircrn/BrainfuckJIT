# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/elvir/CLionProjects/BrainfuckJIT

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/elvir/CLionProjects/BrainfuckJIT/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/BrainfuckJIT.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/BrainfuckJIT.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/BrainfuckJIT.dir/flags.make

CMakeFiles/BrainfuckJIT.dir/main.cpp.o: CMakeFiles/BrainfuckJIT.dir/flags.make
CMakeFiles/BrainfuckJIT.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/elvir/CLionProjects/BrainfuckJIT/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/BrainfuckJIT.dir/main.cpp.o"
	/usr/bin/clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/BrainfuckJIT.dir/main.cpp.o -c /Users/elvir/CLionProjects/BrainfuckJIT/main.cpp

CMakeFiles/BrainfuckJIT.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/BrainfuckJIT.dir/main.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/elvir/CLionProjects/BrainfuckJIT/main.cpp > CMakeFiles/BrainfuckJIT.dir/main.cpp.i

CMakeFiles/BrainfuckJIT.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/BrainfuckJIT.dir/main.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/elvir/CLionProjects/BrainfuckJIT/main.cpp -o CMakeFiles/BrainfuckJIT.dir/main.cpp.s

# Object files for target BrainfuckJIT
BrainfuckJIT_OBJECTS = \
"CMakeFiles/BrainfuckJIT.dir/main.cpp.o"

# External object files for target BrainfuckJIT
BrainfuckJIT_EXTERNAL_OBJECTS =

BrainfuckJIT: CMakeFiles/BrainfuckJIT.dir/main.cpp.o
BrainfuckJIT: CMakeFiles/BrainfuckJIT.dir/build.make
BrainfuckJIT: CMakeFiles/BrainfuckJIT.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/elvir/CLionProjects/BrainfuckJIT/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable BrainfuckJIT"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/BrainfuckJIT.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/BrainfuckJIT.dir/build: BrainfuckJIT

.PHONY : CMakeFiles/BrainfuckJIT.dir/build

CMakeFiles/BrainfuckJIT.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/BrainfuckJIT.dir/cmake_clean.cmake
.PHONY : CMakeFiles/BrainfuckJIT.dir/clean

CMakeFiles/BrainfuckJIT.dir/depend:
	cd /Users/elvir/CLionProjects/BrainfuckJIT/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/elvir/CLionProjects/BrainfuckJIT /Users/elvir/CLionProjects/BrainfuckJIT /Users/elvir/CLionProjects/BrainfuckJIT/cmake-build-debug /Users/elvir/CLionProjects/BrainfuckJIT/cmake-build-debug /Users/elvir/CLionProjects/BrainfuckJIT/cmake-build-debug/CMakeFiles/BrainfuckJIT.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/BrainfuckJIT.dir/depend

