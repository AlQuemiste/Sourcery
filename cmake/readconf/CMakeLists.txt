cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

include(findfile.cmake)

set(libname "cerf")
set(paths " ")

findPkgVersion(${libname} "" ${paths})

message("${libname}_VERSION = ${${libname}_VERSION}")
