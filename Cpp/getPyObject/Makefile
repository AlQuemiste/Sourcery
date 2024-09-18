PYTHON_INCLUDE_DIR := /usr/include/python3.11
PYTHON_LIB := python3.11

ARRAY := get_pyArray
ARRAY_SRC := ./${ARRAY}.cpp
ARRAY_EXE := ./${ARRAY}.exe

TUPLE := get_pyTuple
TUPLE_SRC := ./${TUPLE}.cpp
TUPLE_EXE := ./${TUPLE}.exe

array:
	g++ -std=c++17 -Wall -Wpedantic -I${PYTHON_INCLUDE_DIR} ${ARRAY_SRC} -l${PYTHON_LIB} -o ${ARRAY_EXE}

tuple:
	g++ -std=c++17 -Wall -Wpedantic -I${PYTHON_INCLUDE_DIR} ${TUPLE_SRC} -l${PYTHON_LIB} -o ${TUPLE_EXE}

run_array:
	PYTHONPATH="." ${ARRAY_EXE}

run_tuple:
	PYTHONPATH="." ${TUPLE_EXE}

clean:
	-rm *.exe
	-rm -r __pycache__
