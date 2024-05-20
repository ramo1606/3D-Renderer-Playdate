# Set minimum required CMake version
cmake_minimum_required(VERSION 3.14)

# Set C language standard to C99 (for compatibility with Playdate SDK)
set(CMAKE_C_STANDARD 99)

# --- Playdate SDK Path Resolution ---
# Check for environment variable PLAYDATE_SDK_PATH, which can be set by the user
set(ENVSDK $ENV{PLAYDATE_SDK_PATH})

# If environment variable is not set, try to get it from Playdate config file
if (NOT ${ENVSDK} STREQUAL "")
  # Convert path to CMake format (important if building on Windows)
  file(TO_CMAKE_PATH ${ENVSDK} SDK)
else()
  # Execute shell commands to extract SDK path from config file
  execute_process(
    COMMAND bash -c "egrep '^\\s*SDKRoot' $HOME/.Playdate/config"
    COMMAND head -n 1  # Get the first line
    COMMAND cut -c9-   # Remove the 'SDKRoot=' prefix
    OUTPUT_VARIABLE SDK
    OUTPUT_STRIP_TRAILING_WHITESPACE  # Remove any extra spaces
  )
endif()

# Throw an error if the SDK path couldn't be determined
if (NOT EXISTS ${SDK})
  message(FATAL_ERROR "SDK Path not found; set ENV value PLAYDATE_SDK_PATH")
  return() 
endif()


# --- Project Configuration ---
# Set available build configurations
set(CMAKE_CONFIGURATION_TYPES "Debug;Release") 
# Generate Xcode scheme (for easier building in Xcode)
set(CMAKE_XCODE_GENERATE_SCHEME TRUE) 

# Customize game name and device target name
set(PLAYDATE_GAME_NAME Rasterizer)     
set(PLAYDATE_GAME_DEVICE Rasterizer_DEVICE)

# Define the project (C and Assembly languages used)
project(${PLAYDATE_GAME_NAME} C ASM) 

# --- Include Directories ---
# Tell compiler where to find project headers
include_directories(include)
# Include Playdate SDK C API headers
include_directories(${SDK}/C_API)


# --- Source Files and Executable ---
# Collect all source files (recursively)
file(GLOB_RECURSE SOURCE_FILES src/*.c)
file(GLOB_RECURSE HEADER_FILES include/*.h) 

# Build either executable or library depending on the toolchain
if (TOOLCHAIN STREQUAL "armgcc") # Building for Playdate device
  add_executable(${PLAYDATE_GAME_DEVICE} ${SDK}/C_API/buildsupport/setup.c ${SOURCE_FILES} ${HEADER_FILES})
else() # Building for testing or other platforms
  add_library(${PLAYDATE_GAME_NAME} SHARED ${SOURCE_FILES} ${HEADER_FILES}) 
endif()


# --- Unit Tests (Optional) ---
# (Uncomment and adjust if you're using a C unit testing framework)
# file(GLOB_RECURSE TEST_SOURCES tests/*.c)
# add_executable(unit_tests ${TEST_SOURCES})

# (Uncomment and adjust to link test executable to the game and Playdate stub library)
# target_link_libraries(unit_tests ${PLAYDATE_GAME_NAME} ${SDK}/C_API/buildsupport/pd_api_stub.c) 


# --- Playdate Integration ---
# Include the Playdate-specific CMake module for build configuration
include(${SDK}/C_API/buildsupport/playdate_game.cmake) 
# (Uncomment and adjust if you're ready to build a Playdate game package)
# playdate_game(${PLAYDATE_GAME_DEVICE} ${PLAYDATE_GAME_NAME}) 
