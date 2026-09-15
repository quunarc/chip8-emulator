cmake_minimum_required(VERSION 3.15)
project(main)

# For debugging
# set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pg")
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")

string(ASCII 27 Esc)
set(Red         "${Esc}[31m")
set(Green       "${Esc}[32m")
set(Yellow      "${Esc}[33m")
set(Blue        "${Esc}[34m")
set(ColourReset "${Esc}[m")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(source_dir src)

file(GLOB_RECURSE sources ${source_dir}/*.cpp ${source_dir}/*.c ${source_dir}/*.h)

add_executable(main ${sources})

target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/src/utils)

option(DEBUG_TEST "Enable debug test mode" OFF)

if(DEBUG_TEST)
    message(STATUS "${Red}Debug test mode is ON${ColourReset}")
    target_compile_definitions(main PRIVATE DEBUG_TEST)
endif()
