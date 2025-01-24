cmake_minimum_required(VERSION 3.20)
project(CompressionTest CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find zlib
set(ZLIB_USE_STATIC_LIBS ON)
find_package(ZLIB REQUIRED)

# find BZip2
set(BZIP2_USE_STATIC_LIBS ON)
set(BZip2_USE_STATIC_LIBS ON)
find_package(BZip2 REQUIRED)

# executable
add_executable(ztest ztest.cpp)

# includes for zlib and bzip2
target_include_directories(ztest PRIVATE ${ZLIB_INCLUDE_DIRS} ${BZIP2_INCLUDE_DIRS})

# link against zlib and bzip2
target_link_libraries(ztest PRIVATE ZLIB::ZLIB BZip2::BZip2)

# platform-specific configurations (if any)
if(WIN32)
    message(STATUS "Windows")
elseif(APPLE)
    message(STATUS "MacOS")
elseif(UNIX AND NOT APPLE)
    message(STATUS "Linux")
endif()
