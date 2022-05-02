.PHONY: clean all api api_sep

CC=gcc
CXX=g++
LINK=gcc

CXX_INCLUDES=-I/usr/include/python3.9
CXX_LINKS=-lpython3.9

CXX_CFLAGS=-Wall -Wextra -Wpedantic  ${CXX_INCLUDES}
CXX_LFLAGS= ${CXX_LINKS}  # -Wl,-verbose
CPP_CFLAGS=-std=c++17

SWIG=swig

all: api

example_wrap.cpp: example.i example.h
	@echo "Making SWIG Python API for 'example'..."
	${SWIG} -Wall -python -c++ -o example_wrap.cpp example.i


api: example.cpp example.h example_wrap.cpp
	@echo "Making shared library 'example' with Python API..."
	${CXX} ${CPP_CFLAGS} -fPIC -c example.cpp example_wrap.cpp ${CXX_CFLAGS}
	${CXX} -shared example.o example_wrap.o -o _example.so

api_sep: example.cpp example.h example_wrap.cpp 
	@echo "Making shared library 'example'..."
	${CXX} ${CPP_CFLAGS} -fPIC -c example.cpp example_wrap.cpp ${CXX_CFLAGS}
	${CXX} -shared example.o -o libexample.so
	${CXX} -shared -o _example.so example_wrap.o -L. -lexample '-Wl,--disable-new-dtags,-rpath,$$ORIGIN'
	
#'

clean:
	@echo "Cleaning all products..."
	-rm *.o *.out *.exe example.py example_wrap.cpp *.so
