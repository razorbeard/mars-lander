cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(MarsLander)

# Setup Directories
SET(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

# Add External Dependencies
include(FetchContent)
set(BUILD_SHARED_LIBS OFF)
set(SFML_BUILD_NETWORK OFF)
FetchContent_Declare(
   SFML
   GIT_REPOSITORY https://github.com/SFML/SFML.git
   GIT_TAG 2.6.x
)
FetchContent_MakeAvailable(SFML)

# Copy the resources folder in the same location of the executable
add_custom_target(
    copy_resources ALL COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/resources"
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/resources
)

# Creating Executable
file(GLOB_RECURSE SOURCES src/*.cpp "include/*.hpp")
add_executable(MARS_LANDER ${SOURCES})
target_include_directories(MARS_LANDER PUBLIC "include")
target_link_libraries(MARS_LANDER PRIVATE sfml-graphics)

set_target_properties(
    MARS_LANDER
    PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$(Configuration)
)

add_dependencies(MARS_LANDER copy_resources)
