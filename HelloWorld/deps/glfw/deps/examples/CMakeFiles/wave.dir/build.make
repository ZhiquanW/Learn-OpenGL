# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/samsung2t/Learn-OpenGL/HelloWorld

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/samsung2t/Learn-OpenGL/HelloWorld/build

# Include any dependencies generated for this target.
include ../deps/glfw/deps/examples/CMakeFiles/wave.dir/depend.make

# Include the progress variables for this target.
include ../deps/glfw/deps/examples/CMakeFiles/wave.dir/progress.make

# Include the compile flags for this target's objects.
include ../deps/glfw/deps/examples/CMakeFiles/wave.dir/flags.make

../deps/glfw/deps/examples/CMakeFiles/wave.dir/wave.c.o: ../deps/glfw/deps/examples/CMakeFiles/wave.dir/flags.make
../deps/glfw/deps/examples/CMakeFiles/wave.dir/wave.c.o: ../deps/glfw/examples/wave.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/samsung2t/Learn-OpenGL/HelloWorld/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object ../deps/glfw/deps/examples/CMakeFiles/wave.dir/wave.c.o"
	cd /mnt/samsung2t/Learn-OpenGL/HelloWorld/deps/glfw/deps/examples && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/wave.dir/wave.c.o -c /mnt/samsung2t/Learn-OpenGL/HelloWorld/deps/glfw/examples/wave.c

../deps/glfw/deps/examples/CMakeFiles/wave.dir/wave.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/wave.dir/wave.c.i"
	cd /mnt/samsung2t/Learn-OpenGL/HelloWorld/deps/glfw/deps/examples && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/samsung2t/Learn-OpenGL/HelloWorld/deps/glfw/examples/wave.c > CMakeFiles/wave.dir/wave.c.i

../deps/glfw/deps/examples/CMakeFiles/wave.dir/wave.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/wave.dir/wave.c.s"
	cd /mnt/samsung2t/Learn-OpenGL/HelloWorld/deps/glfw/deps/examples && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/samsung2t/Learn-OpenGL/HelloWorld/deps/glfw/examples/wave.c -o CMakeFiles/wave.dir/wave.c.s

../deps/glfw/deps/examples/CMakeFiles/wave.dir/__/glad_gl.c.o: ../deps/glfw/deps/examples/CMakeFiles/wave.dir/flags.make
../deps/glfw/deps/examples/CMakeFiles/wave.dir/__/glad_gl.c.o: ../deps/glfw/deps/glad_gl.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/samsung2t/Learn-OpenGL/HelloWorld/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object ../deps/glfw/deps/examples/CMakeFiles/wave.dir/__/glad_gl.c.o"
	cd /mnt/samsung2t/Learn-OpenGL/HelloWorld/deps/glfw/deps/examples && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/wave.dir/__/glad_gl.c.o -c /mnt/samsung2t/Learn-OpenGL/HelloWorld/deps/glfw/deps/glad_gl.c

../deps/glfw/deps/examples/CMakeFiles/wave.dir/__/glad_gl.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/wave.dir/__/glad_gl.c.i"
	cd /mnt/samsung2t/Learn-OpenGL/HelloWorld/deps/glfw/deps/examples && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/samsung2t/Learn-OpenGL/HelloWorld/deps/glfw/deps/glad_gl.c > CMakeFiles/wave.dir/__/glad_gl.c.i

../deps/glfw/deps/examples/CMakeFiles/wave.dir/__/glad_gl.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/wave.dir/__/glad_gl.c.s"
	cd /mnt/samsung2t/Learn-OpenGL/HelloWorld/deps/glfw/deps/examples && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/samsung2t/Learn-OpenGL/HelloWorld/deps/glfw/deps/glad_gl.c -o CMakeFiles/wave.dir/__/glad_gl.c.s

# Object files for target wave
wave_OBJECTS = \
"CMakeFiles/wave.dir/wave.c.o" \
"CMakeFiles/wave.dir/__/glad_gl.c.o"

# External object files for target wave
wave_EXTERNAL_OBJECTS =

../deps/glfw/deps/examples/wave: ../deps/glfw/deps/examples/CMakeFiles/wave.dir/wave.c.o
../deps/glfw/deps/examples/wave: ../deps/glfw/deps/examples/CMakeFiles/wave.dir/__/glad_gl.c.o
../deps/glfw/deps/examples/wave: ../deps/glfw/deps/examples/CMakeFiles/wave.dir/build.make
../deps/glfw/deps/examples/wave: ../deps/glfw/deps/src/libglfw3.a
../deps/glfw/deps/examples/wave: /usr/lib/x86_64-linux-gnu/libm.so
../deps/glfw/deps/examples/wave: /usr/lib/x86_64-linux-gnu/librt.so
../deps/glfw/deps/examples/wave: /usr/lib/x86_64-linux-gnu/libm.so
../deps/glfw/deps/examples/wave: /usr/lib/x86_64-linux-gnu/libX11.so
../deps/glfw/deps/examples/wave: ../deps/glfw/deps/examples/CMakeFiles/wave.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/samsung2t/Learn-OpenGL/HelloWorld/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable wave"
	cd /mnt/samsung2t/Learn-OpenGL/HelloWorld/deps/glfw/deps/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/wave.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
../deps/glfw/deps/examples/CMakeFiles/wave.dir/build: ../deps/glfw/deps/examples/wave

.PHONY : ../deps/glfw/deps/examples/CMakeFiles/wave.dir/build

../deps/glfw/deps/examples/CMakeFiles/wave.dir/clean:
	cd /mnt/samsung2t/Learn-OpenGL/HelloWorld/deps/glfw/deps/examples && $(CMAKE_COMMAND) -P CMakeFiles/wave.dir/cmake_clean.cmake
.PHONY : ../deps/glfw/deps/examples/CMakeFiles/wave.dir/clean

../deps/glfw/deps/examples/CMakeFiles/wave.dir/depend:
	cd /mnt/samsung2t/Learn-OpenGL/HelloWorld/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/samsung2t/Learn-OpenGL/HelloWorld /mnt/samsung2t/Learn-OpenGL/HelloWorld/deps/glfw/examples /mnt/samsung2t/Learn-OpenGL/HelloWorld/build /mnt/samsung2t/Learn-OpenGL/HelloWorld/deps/glfw/deps/examples /mnt/samsung2t/Learn-OpenGL/HelloWorld/deps/glfw/deps/examples/CMakeFiles/wave.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : ../deps/glfw/deps/examples/CMakeFiles/wave.dir/depend

