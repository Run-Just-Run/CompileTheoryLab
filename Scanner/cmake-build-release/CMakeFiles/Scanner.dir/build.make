# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.21

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "D:\JETBRAIN\CLion 2021.3.3\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "D:\JETBRAIN\CLion 2021.3.3\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\HP\CLionProjects\Scanner

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\HP\CLionProjects\Scanner\cmake-build-release

# Include any dependencies generated for this target.
include CMakeFiles/Scanner.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Scanner.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Scanner.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Scanner.dir/flags.make

CMakeFiles/Scanner.dir/src/main.cpp.obj: CMakeFiles/Scanner.dir/flags.make
CMakeFiles/Scanner.dir/src/main.cpp.obj: CMakeFiles/Scanner.dir/includes_CXX.rsp
CMakeFiles/Scanner.dir/src/main.cpp.obj: ../src/main.cpp
CMakeFiles/Scanner.dir/src/main.cpp.obj: CMakeFiles/Scanner.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\HP\CLionProjects\Scanner\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Scanner.dir/src/main.cpp.obj"
	"D:\JETBRAIN\CLion 2021.3.3\bin\mingw\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Scanner.dir/src/main.cpp.obj -MF CMakeFiles\Scanner.dir\src\main.cpp.obj.d -o CMakeFiles\Scanner.dir\src\main.cpp.obj -c C:\Users\HP\CLionProjects\Scanner\src\main.cpp

CMakeFiles/Scanner.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Scanner.dir/src/main.cpp.i"
	"D:\JETBRAIN\CLion 2021.3.3\bin\mingw\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\HP\CLionProjects\Scanner\src\main.cpp > CMakeFiles\Scanner.dir\src\main.cpp.i

CMakeFiles/Scanner.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Scanner.dir/src/main.cpp.s"
	"D:\JETBRAIN\CLion 2021.3.3\bin\mingw\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\HP\CLionProjects\Scanner\src\main.cpp -o CMakeFiles\Scanner.dir\src\main.cpp.s

CMakeFiles/Scanner.dir/src/Scanner.cpp.obj: CMakeFiles/Scanner.dir/flags.make
CMakeFiles/Scanner.dir/src/Scanner.cpp.obj: CMakeFiles/Scanner.dir/includes_CXX.rsp
CMakeFiles/Scanner.dir/src/Scanner.cpp.obj: ../src/Scanner.cpp
CMakeFiles/Scanner.dir/src/Scanner.cpp.obj: CMakeFiles/Scanner.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\HP\CLionProjects\Scanner\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Scanner.dir/src/Scanner.cpp.obj"
	"D:\JETBRAIN\CLion 2021.3.3\bin\mingw\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Scanner.dir/src/Scanner.cpp.obj -MF CMakeFiles\Scanner.dir\src\Scanner.cpp.obj.d -o CMakeFiles\Scanner.dir\src\Scanner.cpp.obj -c C:\Users\HP\CLionProjects\Scanner\src\Scanner.cpp

CMakeFiles/Scanner.dir/src/Scanner.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Scanner.dir/src/Scanner.cpp.i"
	"D:\JETBRAIN\CLion 2021.3.3\bin\mingw\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\HP\CLionProjects\Scanner\src\Scanner.cpp > CMakeFiles\Scanner.dir\src\Scanner.cpp.i

CMakeFiles/Scanner.dir/src/Scanner.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Scanner.dir/src/Scanner.cpp.s"
	"D:\JETBRAIN\CLion 2021.3.3\bin\mingw\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\HP\CLionProjects\Scanner\src\Scanner.cpp -o CMakeFiles\Scanner.dir\src\Scanner.cpp.s

CMakeFiles/Scanner.dir/src/Log.cpp.obj: CMakeFiles/Scanner.dir/flags.make
CMakeFiles/Scanner.dir/src/Log.cpp.obj: CMakeFiles/Scanner.dir/includes_CXX.rsp
CMakeFiles/Scanner.dir/src/Log.cpp.obj: ../src/Log.cpp
CMakeFiles/Scanner.dir/src/Log.cpp.obj: CMakeFiles/Scanner.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\HP\CLionProjects\Scanner\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Scanner.dir/src/Log.cpp.obj"
	"D:\JETBRAIN\CLion 2021.3.3\bin\mingw\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Scanner.dir/src/Log.cpp.obj -MF CMakeFiles\Scanner.dir\src\Log.cpp.obj.d -o CMakeFiles\Scanner.dir\src\Log.cpp.obj -c C:\Users\HP\CLionProjects\Scanner\src\Log.cpp

CMakeFiles/Scanner.dir/src/Log.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Scanner.dir/src/Log.cpp.i"
	"D:\JETBRAIN\CLion 2021.3.3\bin\mingw\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\HP\CLionProjects\Scanner\src\Log.cpp > CMakeFiles\Scanner.dir\src\Log.cpp.i

CMakeFiles/Scanner.dir/src/Log.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Scanner.dir/src/Log.cpp.s"
	"D:\JETBRAIN\CLion 2021.3.3\bin\mingw\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\HP\CLionProjects\Scanner\src\Log.cpp -o CMakeFiles\Scanner.dir\src\Log.cpp.s

# Object files for target Scanner
Scanner_OBJECTS = \
"CMakeFiles/Scanner.dir/src/main.cpp.obj" \
"CMakeFiles/Scanner.dir/src/Scanner.cpp.obj" \
"CMakeFiles/Scanner.dir/src/Log.cpp.obj"

# External object files for target Scanner
Scanner_EXTERNAL_OBJECTS =

Scanner.exe: CMakeFiles/Scanner.dir/src/main.cpp.obj
Scanner.exe: CMakeFiles/Scanner.dir/src/Scanner.cpp.obj
Scanner.exe: CMakeFiles/Scanner.dir/src/Log.cpp.obj
Scanner.exe: CMakeFiles/Scanner.dir/build.make
Scanner.exe: vendor/spdlog-1.x/libspdlog.a
Scanner.exe: CMakeFiles/Scanner.dir/linklibs.rsp
Scanner.exe: CMakeFiles/Scanner.dir/objects1.rsp
Scanner.exe: CMakeFiles/Scanner.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\HP\CLionProjects\Scanner\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable Scanner.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\Scanner.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Scanner.dir/build: Scanner.exe
.PHONY : CMakeFiles/Scanner.dir/build

CMakeFiles/Scanner.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\Scanner.dir\cmake_clean.cmake
.PHONY : CMakeFiles/Scanner.dir/clean

CMakeFiles/Scanner.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\HP\CLionProjects\Scanner C:\Users\HP\CLionProjects\Scanner C:\Users\HP\CLionProjects\Scanner\cmake-build-release C:\Users\HP\CLionProjects\Scanner\cmake-build-release C:\Users\HP\CLionProjects\Scanner\cmake-build-release\CMakeFiles\Scanner.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Scanner.dir/depend

