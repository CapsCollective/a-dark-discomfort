# Define custom functions
rwildcard = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
platformpth = $(subst /,$(PATHSEP),$1)

# Set global macros
buildDir := bin
compileFlags := -Wall -std=c++17 -I ./include -I ./engine
linkFlags = -L lib/$(platform) -l raylib -l engine
ifdef MACRO_DEFS
	macroDefines := -D $(MACRO_DEFS)
endif

# Set src target macros
target := $(buildDir)/app
srcDir := src
srcBuildDir := $(buildDir)/$(srcDir)
sources := $(call rwildcard,$(srcDir)/,*.cpp)
objects := $(patsubst $(srcDir)/%, $(srcBuildDir)/%, $(patsubst %.cpp, %.o, $(sources)))
depends := $(patsubst %.o, %.d, $(objects))

# Set engine target macros
engineTarget = lib/$(platform)/libengine.a
engineDir := engine
engineBuildDir := $(buildDir)/$(engineDir)
engineSources := $(call rwildcard,$(engineDir)/,*.cpp)
engineObjects := $(patsubst $(engineDir)/%, $(engineBuildDir)/%, $(patsubst %.cpp, %.o, $(engineSources)))
engineDepends := $(patsubst %.o, %.d, $(engineObjects))

# Set test target macros
testTarget := $(buildDir)/test
testDir := tests
testBuildDir := $(buildDir)/$(testDir)
testSources := $(call rwildcard,$(testDir),*.cpp)
testObjects := $(patsubst $(testDir)/%, $(testBuildDir)/%, $(patsubst %.cpp, %.o, $(testSources)))
testDepends := $(patsubst %.o, %.d, $(testObjects))

# Check for Windows
ifeq ($(OS), Windows_NT)
	# Set Windows macros
	platform := Windows
	CXX ?= g++
	linkFlags += -Wl,--allow-multiple-definition -pthread -lopengl32 -lgdi32 -lwinmm -mwindows -static -static-libgcc -static-libstdc++
	libGenDir := src
	THEN := &&
	PATHSEP := \$(BLANK)
	MKDIR := -mkdir -p
	RM := -del /q
	COPY = -robocopy "$(call platformpth,$1)" "$(call platformpth,$2)" $3
else
	# Check for MacOS/Linux
	UNAMEOS := $(shell uname)
	ifeq ($(UNAMEOS), Linux)
		# Set Linux macros
		platform := Linux
		CXX ?= g++
		linkFlags += -l GL -l m -l pthread -l dl -l rt -l X11
	endif
	ifeq ($(UNAMEOS), Darwin)
		# Set macOS macros
		platform := macOS
		CXX ?= clang++
		linkFlags += -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
		libGenDir := src
	endif

	# Set UNIX macros
	THEN := ;
	PATHSEP := /
	MKDIR := mkdir -p
	RM := rm -rf
	COPY = cp $1$(PATHSEP)$3 $2
endif

# Lists phony targets for Makefile
.PHONY: all setup submodules execute test executeTests cleanTests clean

# Default target, compiles, executes and cleans
all: $(target) execute clean

# Sets up the project for compiling, generates includes and libs
setup: include lib

# Pull and update the the build submodules
submodules:
	git submodule update --init --recursive

# Copy the relevant header files into includes
include: submodules
	$(MKDIR) $(call platformpth, ./include/raylib)
	$(call COPY,vendor/raylib-cpp/vendor/raylib/src,./include/raylib,raylib.h)
	$(call COPY,vendor/raylib-cpp/vendor/raylib/src,./include/raylib,raymath.h)
	$(call COPY,vendor/raylib-cpp/include,./include/raylib,*.hpp)

# Build the raylib static library file and copy it into lib
lib: submodules
	cd vendor/raylib-cpp/vendor/raylib/src $(THEN) "$(MAKE)" PLATFORM=PLATFORM_DESKTOP
	$(MKDIR) $(call platformpth, lib/$(platform))
	$(call COPY,vendor/raylib-cpp/vendor/raylib/$(libGenDir),lib/$(platform),libraylib.a)

# Link the engine and create the static lib
lib/%/libengine.a: $(engineObjects)
	ar -r $(engineTarget) $(engineObjects)

# Add all rules from dependency files
-include $(engineDepends)

# Compile objects to the build directory
$(engineBuildDir)/%.o: $(engineDir)/%.cpp Makefile
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) -MMD -MP -c $(compileFlags) $< -o $@ $(macroDefines)

# Link the program and create the executable
$(target): $(engineTarget) $(objects)
	$(CXX) $(objects) -o $(target) $(linkFlags)

# Add all rules from dependency files
-include $(depends)

# Compile objects to the build directory
$(srcBuildDir)/%.o: $(srcDir)/%.cpp Makefile
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) -MMD -MP -c $(compileFlags) $< -o $@ $(macroDefines)

# Run the executable
execute:
	$(target) $(ARGS)

# Run all unit tests
test: $(testTarget) executeTests cleanTests

# Link the tests and create the executable
$(testTarget): $(engineTarget) $(testObjects)
	$(CXX) $(testObjects) -o $(testTarget) $(linkFlags)

# Add all rules from dependency files
-include $(testDepends)

# Compile test objects to the build directory
$(testBuildDir)/%.o: $(testDir)/%.cpp
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) -MMD -MP -c $(compileFlags) $< -o $@ $(macroDefines)

# Run the test executable
executeTests:
	$(testTarget)

# Clean up all relevant test files
cleanTests:
	$(RM) $(call platformpth, $(testBuildDir)/*)

# Clean up all relevant files
clean:
	$(RM) $(call platformpth, $(buildDir)/*)
