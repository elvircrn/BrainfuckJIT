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
include CMakeFiles/STATIC.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/STATIC.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/STATIC.dir/flags.make

CMakeFiles/STATIC.dir/main.cpp.o: CMakeFiles/STATIC.dir/flags.make
CMakeFiles/STATIC.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/elvir/CLionProjects/BrainfuckJIT/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/STATIC.dir/main.cpp.o"
	/usr/bin/clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/STATIC.dir/main.cpp.o -c /Users/elvir/CLionProjects/BrainfuckJIT/main.cpp

CMakeFiles/STATIC.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/STATIC.dir/main.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/elvir/CLionProjects/BrainfuckJIT/main.cpp > CMakeFiles/STATIC.dir/main.cpp.i

CMakeFiles/STATIC.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/STATIC.dir/main.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/elvir/CLionProjects/BrainfuckJIT/main.cpp -o CMakeFiles/STATIC.dir/main.cpp.s

# Object files for target STATIC
STATIC_OBJECTS = \
"CMakeFiles/STATIC.dir/main.cpp.o"

# External object files for target STATIC
STATIC_EXTERNAL_OBJECTS =

libSTATIC.a: CMakeFiles/STATIC.dir/main.cpp.o
libSTATIC.a: CMakeFiles/STATIC.dir/build.make
libSTATIC.a: CMakeFiles/STATIC.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/elvir/CLionProjects/BrainfuckJIT/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libSTATIC.a"
	$(CMAKE_COMMAND) -P CMakeFiles/STATIC.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/STATIC.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/STATIC.dir/build: libSTATIC.a

.PHONY : CMakeFiles/STATIC.dir/build

CMakeFiles/STATIC.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/STATIC.dir/cmake_clean.cmake
.PHONY : CMakeFiles/STATIC.dir/clean

CMakeFiles/STATIC.dir/depend:
	cd /Users/elvir/CLionProjects/BrainfuckJIT/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/elvir/CLionProjects/BrainfuckJIT /Users/elvir/CLionProjects/BrainfuckJIT /Users/elvir/CLionProjects/BrainfuckJIT/cmake-build-debug /Users/elvir/CLionProjects/BrainfuckJIT/cmake-build-debug /Users/elvir/CLionProjects/BrainfuckJIT/cmake-build-debug/CMakeFiles/STATIC.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/STATIC.dir/depend

