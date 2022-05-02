.PHONY: all clean py

all: py


CC=gcc
CXX=g++
LINK=gcc

CXX_INCLUDES=-I/usr/include/python3.9
CXX_LINKS=-lpython3.9

CXX_CFLAGS=-Wall -Wextra -Wpedantic -fsanitize=undefined ${CXX_INCLUDES}
CXX_LFLAGS= ${CXX_LINKS}  # -Wl,-verbose
CPP_CFLAGS=-std=c++17

py: PythonInterpreter.cpp PythonInterpreter.h PyObjectPtr.cpp PyObjectDecl.h
	@echo "Making an embedded Python interpreter..."
	${CXX} -c -fPIC PythonInterpreter.cpp PyObjectPtr.cpp ${CPP_CFLAGS} ${CXX_CFLAGS}
	${CXX} -shared -o libpyintrp.so PythonInterpreter.o PyObjectPtr.o ${CXX_CFLAGS}
	${CXX} -o pyintrp.exe pythonintrp_main.cpp ${CXX_CFLAGS} ${CPP_CFLAGS} -L. -lpyintrp ${CXX_LFLAGS} '-Wl,--disable-new-dtags,-rpath,$$ORIGIN'


clean:
	@echo "Cleaning all products..."
	rm *.o *.out *.exe
