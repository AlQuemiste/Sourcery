PYTHON_INCLUDE_DIR := /usr/include/python3.11
PYTHON_LIB := python3.11
FLAGS := -std=c++17 -Wall -Wpedantic

ARRAY := get_pyArray
ARRAY_SRC := ./${ARRAY}.cpp
ARRAY_EXE := ./${ARRAY}.exe

TUPLE := get_pyTuple
TUPLE_SRC := ./${TUPLE}.cpp
TUPLE_EXE := ./${TUPLE}.exe

FUNC := get_pyFunction
FUNC_SRC := ./${FUNC}.cpp
FUNC_EXE := ./${FUNC}.exe

array:
	g++ ${FLAGS} -I${PYTHON_INCLUDE_DIR} ${ARRAY_SRC} -l${PYTHON_LIB} -o ${ARRAY_EXE}

tuple:
	g++ ${FLAGS} -I${PYTHON_INCLUDE_DIR} ${TUPLE_SRC} -l${PYTHON_LIB} -o ${TUPLE_EXE}

function:
	g++ ${FLAGS} -I${PYTHON_INCLUDE_DIR} ${FUNC_SRC} -l${PYTHON_LIB} -o ${FUNC_EXE}

run_array:
	PYTHONPATH="." ${ARRAY_EXE}

run_tuple:
	PYTHONPATH="." ${TUPLE_EXE}

run_function:
	PYTHONPATH="." ${FUNC_EXE}

clean:
	-rm *.exe
	-rm -r __pycache__
