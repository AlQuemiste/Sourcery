# see <https://cmake.org/pipermail/cmake/2015-May/060698.html>
cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(FunctionsPlayground)

set(name Fit)
set(lib Lib${name})

# --- source and include files ---

file(GLOB ${lib}_source_files /home/ammar/Projects/bornagain/Base/Axis/*.cpp)
file(GLOB ${lib}_include_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} */*.h)

set(mylist "A;B;C")
set(mylist2 mylist)
foreach(it ${mylist})
    set(it2 ${it}_1_2_3)
    list(APPEND mylist2 ${it2})
    message("'${it}' -> '${it2}'")
endforeach()

message("mylist2 = ${mylist2}")

set(Boost_LIBRARIES_wheel "/usr/lib/x86_64-linux-gnu/libboost_iostreams.so;/usr/lib/x86_64-linux-gnu/libboost_program_options.so;/usr/lib/x86_64-linux-gnu/libboost_regex.so")

list(FILTER Boost_LIBRARIES_wheel INCLUDE REGEX ".*io.?stream.*")
message("Boost_LIBRARIES_wheel = {${Boost_LIBRARIES_wheel}}")
