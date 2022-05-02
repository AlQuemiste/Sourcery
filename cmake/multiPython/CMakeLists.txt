cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(findMultiplePythons)
# usage:
# $ cmake -S . -B build
# for debugging, add `--debug-find`, `--trace-source=CMakeLists.txt` or `--trace-expand`

set(pyminversion 8)
set(pyprefix "py3${pyminversion}")
set(pypath "/home/ammar/Projects/py/multipython/py3.8.12")

include("findMultiPython.cmake")
find_Python3Dev("${pyminversion}" "${pyprefix}" "${pypath}" verbose)

if(NOT ${pyprefix}_Python3_FOUND)
  message(FATAL_ERROR "Python 3.${pyminversion} not found.")
endif()

if(NOT ${pyprefix}_Python3_Development_FOUND)
  message(FATAL_ERROR
    "The development modules for Python 3.${pyminversion} not found.")
endif()

if(NOT ${pyprefix}_Python3_NumPy_FOUND)
  message(FATAL_ERROR "Python3.${pyminversion}-NumPy not found.")
endif()

# include(CMakePackageConfigHelpers)
# write_basic_package_version_file("Foo-ConfigVersion.cmake"
#   VERSION 1.2.3
#   COMPATIBILITY SameMinorVersion
#   )

# See section "Config Mode Version Selection" in `find_package` documentation <https://cmake.org/cmake/help/latest/command/find_package.html?highlight=package_version_exact#config-mode-version-selection>

# DEBUG

# ref <https://stackoverflow.com/q/9298278>
# message(STATUS ">> CMake Variables")
# get_cmake_property(_variableNames VARIABLES)
# list (SORT _variableNames)
# foreach (_variableName ${_variableNames})
#   message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()
# message(STATUS ">> END CMake Variables")

#END  DEBUG
