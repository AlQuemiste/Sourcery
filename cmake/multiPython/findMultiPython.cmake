cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

function(find_Python3Dev version_minor prefix paths verbosity)
  # usage:
  # * find Python 3.8 with verbose mode on:
  #   find_Python3Dev(8 "py38" verbose)
  # * find Python 3.7 with verbose mode off:
  #   find_Python3Dev(7 "py37" quiet)

  set(_msghdr "${CMAKE_CURRENT_FUNCTION}::")   # header for output messages

  set(Python3_ROOT_DIR "${paths}")

  set(CMAKE_FIND_LIBRARY_SUFFIXES .so .a)
  find_package(Python3 "3.${version_minor}" EXACT QUIET
    COMPONENTS Interpreter Development NumPy PATHS "${paths}" NO_DEFAULT_PATH)

  if(NOT Python3_FOUND)
    message(WARNING "${_msghdr} Python 3.${version_minor} not found.")
  endif()

  if(NOT Python3_Development_FOUND)
    message(WARNING
      "${_msghdr} The development modules for Python 3.${version_minor} not found. Probably, you need to install package libpython3-dev (or similar).")
  endif()

  if(NOT Python3_NumPy_FOUND)
    message(WARNING "${_msghdr} Python3.${version_minor}-NumPy not found.")
  endif()

  # define exported parameters to the parent scope
  # eg., with prefix 'py39', 'py39_Python3_LIBRARY_DIRS' will be exported, etc.
  set(prefix_ "${prefix}_")
  set(_exportPrms Python3_FOUND Python3_Development_FOUND Python3_NumPy_FOUND
    Python3_INCLUDE_DIRS Python3_LIBRARY_DIRS Python3_LIBRARIES
    Python3_NumPy_VERSION Python3_NumPy_INCLUDE_DIRS)
  foreach(pyvar IN LISTS _exportPrms)
    set(${prefix_}${pyvar} ${${pyvar}} PARENT_SCOPE)
  endforeach()

  string(TOUPPER ${verbosity} verbosity)
  if("${verbosity}" STREQUAL "VERBOSE")
    message(STATUS ">> ${_msghdr} version=3.${version_minor}, prefix='${prefix_}':")
    message(STATUS "   Python3_VERSION: ${Python3_VERSION}")
    message(STATUS "   Python3_VERSION_MINOR: ${Python3_VERSION_MINOR}")
    message(STATUS "   Python3_VERSION_PATCH: ${Python3_VERSION_PATCH}")
    message(STATUS "   Python3_INTERPRETER_ID: ${Python3_INTERPRETER_ID}")
    message(STATUS "   Python3_EXECUTABLE: ${Python3_EXECUTABLE}")
    message(STATUS "   Python3_STDLIB: ${Python3_STDLIB}")
    message(STATUS "   Python3_SITELIB: ${Python3_SITELIB}")
    message(STATUS "   Python3_INCLUDE_DIRS: ${Python3_INCLUDE_DIRS}")
    message(STATUS "   Python3_LIBRARIES: ${Python3_LIBRARIES}")
    message(STATUS "   Python3_LIBRARY_DIRS: ${Python3_LIBRARY_DIRS}")
    message(STATUS "   Python3_SOABI: ${Python3_SOABI}")
    message(STATUS "   Python3_NumPy_VERSION: ${Python3_NumPy_VERSION}")
    message(STATUS "   Python3_NumPy_INCLUDE_DIRS: ${Python3_NumPy_INCLUDE_DIRS}")
    message(STATUS "   Exported parameters prefixed with '${prefix_}': \{${_exportPrms}\}")
  endif()

endfunction()
