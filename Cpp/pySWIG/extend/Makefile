SWIG := /usr/bin/swig
CXX := g++
PYTHON_INCLUDE_DIR := "/usr/include/python3.11/"
module := MyClass

all: api
	${CXX} -fPIC -O3 -Wall -c ${module}.cpp -o ${module}.o
	${CXX} -fPIC -O3 -Wall -c ${module}_wrap.cpp -I${PYTHON_INCLUDE_DIR} -o ${module}_wrap.o
	${CXX} -shared -fPIC -O3 ${module}_wrap.o ${module}.o -o _${module}.so

api:
	${SWIG} -Wall -python -c++ -o ${module}_wrap.cpp ${module}.i

test:
	python3 test_${module}.py

clean:
	-rm *.o *.so ${module}_wrap.cpp ${module}.py
	-rm -r __pycache__
