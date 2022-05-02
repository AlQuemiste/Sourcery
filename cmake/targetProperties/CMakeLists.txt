# see <https://cmake.org/pipermail/cmake/2015-May/060698.html>
cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(CopyTargetProperties)

set(name Fit)
set(lib Lib${name})

# --- source and include files ---

file(GLOB ${lib}_source_files /home/ammar/Projects/bornagain/Base/Axis/*.cpp)
file(GLOB ${lib}_include_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} */*.h)

message("lib = ${lib}")

# --- make the library ---
macro(internalMacro)
  message(">>> This is an internal macro")
  message(">>> internal macro: lib_name = ${lib_name}")
endmacro()

macro(def_${lib} lib_name)
    message("MACRO: lib_name = '${lib_name}'")
    message("MACRO: source_files = '${${lib}_source_files}'")
    internalMacro()

    add_library(${lib_name} OBJECT)
    set_target_properties(${lib_name}
      PROPERTIES
      SOURCES "${${lib}_source_files}"
      INCLUDES "${${lib}_include_files}")

    # --- external dependences ---
    set(link_libs_public "publib1.so;publib2.so")
    set(link_libs_private "prvlib1.so;prvlib2.so")

    set(include_public "pub/inc1;pub/inc2")
    set(include_private "prv/inc1;prv/inc2")

    target_link_libraries(${lib_name}
        PUBLIC
        ${link_libs_public}
        PRIVATE
        ${link_libs_private}
        )

    target_include_directories(${lib_name}
        PUBLIC
        ${CMAKE_SOURCE_DIR}
        SYSTEM PUBLIC # because needed by Fit/Test/Unit
        ${include_public}
        SYSTEM PRIVATE
        ${include_private}
        )
endmacro()

# set(source_files "BLAH")

cmake_language(CALL def_${lib} ${lib})
target_link_libraries(${lib} PUBLIC "ABC.so")
set_target_properties(${lib} PROPERTIES XYZProperty "XYZ")

get_target_property(${lib}_cmp_def ${lib} COMPILE_DEFINITIONS)
get_target_property(${lib}_inc_dir ${lib} INCLUDE_DIRECTORIES)
get_target_property(${lib}_link_libs ${lib} LINK_LIBRARIES)
get_target_property(${lib}_xyz ${lib} XYZProperty)

message("${lib}:COMPILE_DEFINITIONS = ${${lib}_cmp_def}")
message("${lib}:INCLUDE_DIRECTORIES = ${${lib}_inc_dir}")
message("${lib}:LINK_LIBRARIES = ${${lib}_link_libs}")
message("${lib}:XYZProperty = ${${lib}_xyz}")

#-----------------------------------------------------
# arbitrary target with arbitrary properties

add_custom_target(myCustomTarget)
set_target_properties(myCustomTarget PROPERTIES myProperty ON)
get_target_property(prp myCustomTarget myProperty)
message("* myCustomTarget.myProperty = ${prp}")

#-----------------------------------------------------

function(myFn var1 var2)
    set(prefix myFn)
    set(options OPT FAST)
    set(oneValueArgs
        DESTINATION_LIB DESTINATION_INC)
    set(multiValueArgs TARGETS CONF)
    cmake_parse_arguments(${prefix}
        "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    message("${prefix}: var1 = ${var1}, var2 = ${var2}")
    message("${prefix}_OPT: ${${prefix}_OPT}")
    message("${prefix}_FAST: ${${prefix}_FAST}")
    message("${prefix}_DESTINATION_LIB: ${${prefix}_DESTINATION_LIB}")
    message("${prefix}_DESTINATION_LIB: ${${prefix}_DESTINATION_INC}")
    message("${prefix}_TARGETS: ${${prefix}_TARGETS}")
    message("${prefix}_CONF: ${${prefix}_CONF}")

    foreach(var IN LISTS options oneValueArgs multiValueArgs)
        message("${var} = ${${prefix}_${var}}")
    endforeach()
endfunction()

myFn(x1 x2 DESTINATION_INC "a/" TARGETS "a;n;c" OPT)

set(my.var true)
message("my.var = ${my.var}")

#---------------------------------------------------
message("-----")
function(_defprp _libname _objects tagpy)
    # if("${tagpy}" STREQUAL "")
    if(NOT tagpy)
        message("NO tagpy = ${tagpy}")
    else()
        message("tagpy = ${tagpy}")
    endif()
endfunction()


set(_ptag "py37")
_defprp("LIB1" "OBJ1" "${_ptag}")

if(NOT _ptag)
    message("NO _ptag = ${_ptag}")
else()
    message("_ptag = ${_ptag}")
endif()
