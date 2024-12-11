cmake_minimum_required(VERSION 3.12)
project(PythonExtension LANGUAGES CXX C)

include(CTest)

# find Python
find_package(Python 3 COMPONENTS Interpreter Development REQUIRED)

# create the extension module
add_library(pyCore SHARED pycore.cpp)

# include Python headers
target_include_directories(pyCore PUBLIC pycore.hpp PRIVATE ${Python_INCLUDE_DIRS})

message(STATUS "Python3:")
message(STATUS "Python_INCLUDE_DIRS: '${Python_INCLUDE_DIRS}'")
message(STATUS "Python_LIBRARIES: '${Python_LIBRARIES}'")

# link the module with Python libraries
target_link_libraries(pyCore PUBLIC ${Python_LIBRARIES})

# set the output name to match Python extension library naming convention
set_target_properties(pyCore PROPERTIES PREFIX "")
if(WIN32)
    set_target_properties(pyCore PROPERTIES SUFFIX ".pyd")
else()
    set_target_properties(pyCore PROPERTIES SUFFIX ".so")
endif()

# executable
add_executable(testpy testpy.cpp)
target_link_libraries(testpy PRIVATE pyCore)

# tests
add_test(NAME testPython COMMAND ${CMAKE_BUILD_DIR}/testpy)
set_property(TEST testPython PROPERTY ENVIRONMENT "PYTHONPATH=${CMAKE_SOURCE_DIR}")
