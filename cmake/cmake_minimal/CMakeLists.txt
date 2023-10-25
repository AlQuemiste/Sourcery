# usage:
# >>> cmake -S <src-dir> -B <build-dir> -D<var-id>=<value> -Wdev --warn-uninitialized --warn-unused-vars
# >>> cmake --build <build-dir> --clean-first --parallel 4 --target Release
# see <https://cmake.org/cmake/help/latest/manual/cmake.1.html>

# see: https://stackoverflow.com/q/21163188

cmake_minimum_required(VERSION 3.5)
# colourised output <https://stackoverflow.com/a/19578320>
if(NOT WIN32)
  string(ASCII 27 Esc)
  set(ColourReset "${Esc}[m")
  set(ColourBold  "${Esc}[1m")
  set(Red         "${Esc}[31m")
  set(Green       "${Esc}[32m")
  set(Yellow      "${Esc}[33m")
  set(Blue        "${Esc}[34m")
  set(Magenta     "${Esc}[35m")
  set(Cyan        "${Esc}[36m")
  set(White       "${Esc}[37m")
  set(BoldRed     "${Esc}[1;31m")
  set(BoldGreen   "${Esc}[1;32m")
  set(BoldYellow  "${Esc}[1;33m")
  set(BoldBlue    "${Esc}[1;34m")
  set(BoldMagenta "${Esc}[1;35m")
  set(BoldCyan    "${Esc}[1;36m")
  set(BoldWhite   "${Esc}[1;37m")
endif()

project(foo VERSION 1.0.0)
option(OPTIONA "Help-text>>> Option-A is an option" OFF)

message(STATUS "${ColourBold}Project: ${PROJECT_NAME}, ${foo_VERSION}, ${foo_DESCRIPTION}${ColourReset}")

message(STATUS "source dir:" ${foo_SOURCE_DIR})
message(STATUS "bin dir:" ${foo_BINARY_DIR})
message(STATUS "system: " ${CMAKE_SYSTEM_NAME} ", " ${CMAKE_SYSTEM_VERSION})
message(STATUS "compiler: " ${CMAKE_CXX_COMPILER_ID} ", " ${CMAKE_CXX_COMPILER_VERSION}
  " for " ${CMAKE_HOST_SYSTEM_PROCESSOR})

message(STATUS "User set:" ${MYVAR})

set(DEFAULT_BUILD_TYPE "Release")

#return()

# include(GNUInstallDirs)
# set(SOURCE_DIR "/home/utopia/Documents/shlib")
# set(SOURCE_FILES ${SOURCE_DIR}/foo.c)
set(SOURCE_DIR ${foo_SOURCE_DIR})

set(foolibname foo)
add_library(${foolibname} SHARED ${SOURCE_DIR}/foo.c)

add_library(qux SHARED ${SOURCE_DIR}/qux.c)

add_executable(mytest ${SOURCE_DIR}/main.c)
target_link_libraries(mytest PRIVATE ${foolibname} qux)
target_compile_options(mytest PRIVATE -std=c++11 -Wall -Wfloat-conversion)
# target_include_directories(${foolibname} PUBLIC
#     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#     $<INSTALL_INTERFACE:include>
#     PRIVATE src)

# set_target_properties(${foolibname} PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION 3)

get_target_property(LibName ${foolibname} LIBRARY_OUTPUT_NAME)
message(STATUS "LibName: ${LibName}")

file(WRITE ${CMAKE_BINARY_DIR}/readme.py "version='${foolibname}.${PROJECT_VERSION}'; print(version);")

# CHECK THIS vvv
# drop any version info from so-name of the shared-library (eg., `libcefidtools.so`)
# see https://stackoverflow.com/a/53553496/3484761
# set_target_properties(${foolibname} PROPERTIES VERSION ${PROJECT_MAJOR_VERSION}.${PROJECT_MINOR_VERSION}.${PROJECT_PATCH_LEVEL}
#                       SUFFIX "" SOVERSION "so")

# see <https://cmake.org/cmake/help/latest/command/if.html>
if (OPTIONA)  # if OPTIONA is 1, Y, ON, YES, TRUE
  message(STATUS "${BoldBlue}Option-A: ${OPTIONA}${ColourReset}")
endif()

# Boost
find_package(Boost COMPONENTS filesystem unit_test_framework program_options python numpy)

if (Boost_FOUND)
  message(STATUS "Boost found.")
else (Boost_FOUND)
  message(FATAL_ERROR "Boost NOT found!")
endif(Boost_FOUND)
