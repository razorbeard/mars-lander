cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(MARS_LANDER)

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

add_custom_target(
    copy_resources ALL COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/resources
    ${PROJECT_BINARY_DIR}/$<CONFIGURATION>/resources
)

file(GLOB_RECURSE SOURCES src/*.cpp "include/*.hpp")
add_executable(MARS_LANDER ${SOURCES})
add_dependencies(MARS_LANDER copy_resources)
target_include_directories(MARS_LANDER PUBLIC "include")
target_link_libraries(MARS_LANDER PRIVATE sfml-graphics)

install(TARGETS MARS_LANDER)
