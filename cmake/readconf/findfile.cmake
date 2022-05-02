cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

function(findPkgVersion libname conffile paths)

  # find configuration file
  string(STRIP ${paths} _paths)
  if(NOT _paths)
    set(_paths . pkgconfig pkgconf pkg_config pkg_conf)
  endif()

  if(conffile)
    set(_pkgconffile ${conffile})
  else()
    set(_pkgconffile "lib${libname}.pc")
  endif()

  set(${libname}_VERSION "${libname}_VERSION-NOTFOUND" PARENT_SCOPE)

  # NOTE: ${libname}_PATH variable will be cached
  find_path(${libname}_PATH ${_pkgconffile} PATHS ${_paths} DOC "Package configuration for ${_pkgconffile}")

  # NOTE: A CMake value is False if the constant is 0, OFF, NO, FALSE, N, IGNORE, "", or ends in the suffix '-NOTFOUND'.
  if(${libname}_PATH)
    message(STATUS "'${_pkgconffile}' found in '${${libname}_PATH}'")
  else()
    message(WARNING "'${_pkgconffile}' not found in paths {${_paths}}")
    return()
  endif()

  # extract the configuration information
  file(READ "${${libname}_PATH}/${_pkgconffile}" _conf)
  string(TOLOWER ${_conf} _conf_lc)
  string(REGEX MATCH "version[: \t]*([1-9.]+)" _rxmatch ${_conf_lc})

  if(${CMAKE_MATCH_1})
    set(${libname}_VERSION ${CMAKE_MATCH_1} PARENT_SCOPE)
  endif ()

endfunction()
