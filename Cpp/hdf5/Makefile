.PHONY: all clean read write states matrix

CXX=g++ -std=c++17 -Wall -Wpedantic
H5INC=-I/usr/include/hdf5/serial
H5LIB=-L/usr/lib/x86_64-linux-gnu/hdf5/serial/
H5LINKS=-lhdf5 -lhdf5_cpp
EIGENINC=-I/usr/include/eigen3

read:
	${CXX} readh5.cpp ${H5INC} ${H5LIB} ${H5LINKS} -o rh5.exe

write:
	${CXX} writeh5.cpp ${H5INC} ${H5LIB} ${H5LINKS} -o wh5.exe

states:
	${CXX} writestates.cpp ${H5INC} ${H5LIB} ${H5LINKS} -o wst.exe

slab:
	${CXX} test_slab.cpp ${H5INC} ${H5LIB} ${H5LINKS} -o slb.exe

matrix:
	${CXX} writematrix.cpp ${EIGENINC} ${H5INC} ${H5LIB} ${H5LINKS} -o mtx.exe

rowmat:
	${CXX} test_row_mat.cpp ${EIGENINC} ${H5INC} ${H5LIB} ${H5LINKS} -o rmt.exe

clean:
	-rm -v *.exe *.o *.out *.h5
