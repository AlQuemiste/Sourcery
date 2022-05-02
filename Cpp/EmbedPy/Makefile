.PHONY: all clean embedpy1 embedpy2 pyinterp

all: embedpy1 embedpy2


CC=gcc
CXX=g++
LINK=gcc

CXX_INCLUDES=-I/usr/include/python3.9
CXX_LINKS=-lpython3.9

CXX_CFLAGS=-Wall -Wextra -Wpedantic -fsanitize=undefined ${CXX_INCLUDES}
CXX_LFLAGS= ${CXX_LINKS}  # -Wl,-verbose
CPP_CFLAGS=-std=c++17

embednpy: embednpy.cpp
	@echo "Making 'embednpy'..."
	${CXX} -o embednpy.exe embednpy.cpp ${CPP_CFLAGS} ${CXX_CFLAGS} ${CXX_LFLAGS}

embedpy1: embedpy1.c
	@echo "Making 'embedpy1'..."
	${CC} -o embedpy1.exe embedpy1.c ${CXX_CFLAGS} ${CXX_LFLAGS}

embednumpy: embednumpy.cpp
	@echo "Making 'embednumpy'..."
	${CXX} -o embednumpy.exe embednumpy.cpp ${CPP_CFLAGS} ${CXX_CFLAGS} ${CXX_LFLAGS}

embednumpy_macro: embednumpy.cpp
	@echo "Making 'embednumpy'..."
	${CXX} -E -o embednumpy_exmacro.h embednumpy.cpp ${CPP_CFLAGS} ${CXX_CFLAGS}

embedpy2: embedpy2.cpp
	@echo "Making 'embedpy2'..."
	${CXX} -o embedpy2.exe embedpy2.cpp ${CXX_CFLAGS} ${CXX_LFLAGS} ${CPP_CFLAGS}

embedpy3: embedpy3.cpp
	@echo "Making 'embedpy3'..."
	${CXX} -o embedpy3.exe embedpy3.cpp ${CXX_CFLAGS} ${CXX_LFLAGS} ${CPP_CFLAGS}

pyinterp: pythonintrp.cpp
	@echo "Making 'pythonintrp'..."
	${CXX} -c -fPIC pythonintrp.cpp PyObjectPtr.cpp ${CPP_CFLAGS} ${CXX_CFLAGS}
	${CXX} -shared -o libpyintrp.so pythonintrp.o PyObjectPtr.o ${CXX_CFLAGS}
	${CXX} -o pyintrp.exe pythonintrp_main.cpp ${CXX_CFLAGS} ${CPP_CFLAGS} -L. -lpyintrp ${CXX_LFLAGS} '-Wl,--disable-new-dtags,-rpath,$$ORIGIN'


clean:
	@echo "Cleaning all products..."
	rm *.o *.out *.exe
