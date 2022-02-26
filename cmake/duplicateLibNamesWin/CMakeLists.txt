cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(TestDuplicateNamesMSVC LANGUAGES C)

if(MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    # set(BUILD_SHARED_LIBS TRUE)
endif()

set(libname prime)
set(libdir1 ${CMAKE_BINARY_DIR}/lib1)
set(libdir2 ${CMAKE_BINARY_DIR}/lib2)

# NOTE: The output names of both libraries are the _same_,
# yet they are built in _different_ directories

add_library(prime1 SHARED prime1.c)
set_target_properties(prime1 PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    RUNTIME_OUTPUT_NAME ${libname}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${libdir1}
    OUTPUT_NAME ${libname}
    LIBRARY_OUTPUT_DIRECTORY ${libdir1}
    # LINK_FLAGS "/IMPLIB:\"${libdir1}/${libname}.lib\""
    )

add_library(prime2 SHARED prime2.c)
set_target_properties(prime2 PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    RUNTIME_OUTPUT_NAME ${libname}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${libdir2}
    OUTPUT_NAME ${libname}
    LIBRARY_OUTPUT_DIRECTORY ${libdir2}
    # LINK_FLAGS "/IMPLIB:\"${libdir2}/${libname}.lib\""
    )

add_executable(main main.c)
target_link_libraries(main prime1)
target_link_directories(main PRIVATE "${libdir1}")

# cmake -S . -B build
# cmake --build build --config Release
