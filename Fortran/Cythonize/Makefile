.PHONY: meshlib cython

none:

cython: meshlib
	python3 setup.py build_ext --inplace

meshlib:
	gfortran -Wall -fPIC -shared -o meshlib.so -std=f2008 mesh.f08
	
test:
	python3 -c "import fortmath as fm; v = fm.mesh_exp(1, 2, 3, 4); print('v =', v); print('myfoo(1) =', fm.myfoo(1))"

clean:
	-rm *.mod *.so fort.c
	-rm -r build
