############################################################################
# CMakeLists.txt file for building libBornAgainPyCore
############################################################################

# NOTE: The module is directly dependent on Python C-API
if(NOT BA_PYTHON)
    message(FATAL_ERROR "PyCore module requested although BA_PYTHON=false")
endif()

set(component PyCore)
set(lib BornAgain${component})

# --- source and include files ---
# Python-dependent source files
file(GLOB_RECURSE source_files */*.cpp)
file(GLOB_RECURSE include_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} */*.h)

# --- make the library ---

add_library(${lib} SHARED)
MakeLib(${lib} "" "${source_files}" "${include_files}")

# --- external dependences ---

target_link_libraries(${lib}
    PUBLIC
    ${Python3_LIBRARIES}
)
target_include_directories(${lib}
    PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${LibHeinz_INCLUDE_DIR}
    ${Python3_INCLUDE_DIRS}
)

target_compile_options(${lib} PRIVATE $<$<CONFIG:Debug>:${BA_DEBUG_COMPILE_OPTION}>)
