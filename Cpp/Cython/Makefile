.PHONY: cython cython_header embed cython1 test clean


libfoo.so: src/libfoo.cpp src/libfoo.hpp
	g++ -Wall -fPIC -shared -O3 -std=c++17 -o libfoo.so src/libfoo.cpp

cython: src/foothing.pyx libfoo.so
	python3 setup.py

cython_header: src/foothing.pyx
	cython --cplus -3 -v -o foothing.hpp src/foothing.pyx

embed: ./foothing.hpp
	g++ -Wall -std=c++17 -I"/usr/include/python3.9" -I"./buildcy/src" embedCython.cpp ./foothing.cpython-39-x86_64-linux-gnu.so ./libfoo.so -lpython3.9 -o foothing_test.out

cython1: src/foothing.pyx libfoo.so
	python3 setup.py build_ext --inplace

test:
	python3 -c "import fortmath as fm; v = fm.mesh_exp(1, 2, 3, 4); print('v =', v); print('myfoo(1) =', fm.myfoo(1))"

clean:
	-rm *.o *.so *.out
