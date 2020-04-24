#!/bin/bash

# CMake platform-independent commands:
#    <https://cmake.org/cmake/help/latest/manual/cmake.1.html>

SEP="----------------------------------------"
PROC_NR=4  # nr of concurrent build processes
BUILD_DIR='./build'
TARGET=Release
# if a local Boost is to be used,
# use `LOCALBOOST` with `BOOSTDIR` set to the proper directory 
BOOSTDIR="~/.local/lib/boost_1_71_0"
LOCALBOOST=-DBoost_NO_SYSTEM_PATHS=ON \
	  -DBOOST_INCLUDEDIR="${BOOSTDIR}/include" \
	  -DBOOST_LIBRARYDIR="${BOOSTDIR}/lib"

BOOST_DYNLINK=OFF  # Turn ON/OFF dynamic linking for Boost libraries (default = OFF)
PYTHON_SUFFIX=""   # suffix of Boost-Python libraries; eg. "", "3", "36"
# NOTE: To find the directory for Boost library, use eg.,
# >>> locate libboost
# on your Linux system. The _static_ libraries to compile Python and Numpy extensions are named like:
#
# libboost_python.a
# libboost_python3.a
# libboost_python36.a
# libboost_numpy.a
# libboost_numpy3.a
# libboost_numpy36.a

CMAKE_DEBUG_FLAGS='-Wdev --warn-uninitialized --warn-unused-vars'

echo "*** Building Project ***"
echo $SEP

echo "* Build directory:" $BUILD_DIR
mkdir -p $BUILD_DIR
echo $SEP

echo "* CMake: Configuring..."
pushd $BUILD_DIR
cmake .. -DOPTIONA=ON \
      -Wno-dev  # suppress dev. warnings

cmake --build . -- -j $PROC_NR  # VERBOSE=1 --warn-undefined-variables

popd

echo "--- END ---"
echo


