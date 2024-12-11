cmake_minimum_required(VERSION 3.12)
project(PythonExtension LANGUAGES CXX C)

include(CTest)
include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 17)

if (WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    set(BUILD_SHARED_LIBS TRUE)
endif()

# find Python
find_package(Python 3 COMPONENTS Development REQUIRED)

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
    set_target_properties(pyCore PROPERTIES
    SUFFIX ".dll"
    OUTPUT_NAME "pyCore"
    RUNTIME_OUTPUT_NAME "pyCore")
else()
    set_target_properties(pyCore PROPERTIES SUFFIX ".so"  OUTPUT_NAME "pyCore")
endif()

# executable
add_executable(testpy testpy.cpp)
target_link_libraries(testpy PRIVATE pyCore)

# tests
add_test(NAME testPython COMMAND ${CMAKE_BUILD_DIR}/testpy)
set_property(TEST testPython PROPERTY ENVIRONMENT "PYTHONPATH=${CMAKE_SOURCE_DIR}")

# install
install(TARGETS pyCore LIBRARY DESTINATION ${CMAKE_INSTALL_BINDIR})
install(TARGETS testpy RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
