.PHONY: test, test_mixo

# compile and link Fortran library with C- and Python interfaceFortran object

all: libarr_mean.so arr_mean.exe

arr_mean.exe: arr_mean.c libarr_mean.so
	gcc -Wall -o arr_mean.exe arr_mean.c -L. -larr_mean -Wl,-rpath,.

libarr_mean.so: arr_mean.f08
	gfortran -Wall -O3 -fPIC -shared -o libarr_mean.so -std=f2008 arr_mean.f08

test: all
	@echo "* run C-executable:"
	./arr_mean.exe
	@echo "* run Python script:"
	python3 arr_mean.py

# compile and link C-object with Fortran object
mixo: arr_mean_f.o arr_mean_mix.exe

arr_mean_mix.exe: arr_mean.c arr_mean_f.o
	gcc -Wall -o arr_mean_mix.exe arr_mean.c arr_mean_f.o # -lgfortran

arr_mean_f.o: arr_mean.f08
	gfortran -Wall -O3 -std=f2008 -o arr_mean_f.o -c arr_mean.f08

test_mixo: mixo
	@echo "* Check dependencies of the produced executable:"
	ldd ./arr_mean_mix.exe
	@echo
	@echo "* run C-executable made from a mixture of C- and F-objects:"
	./arr_mean_mix.exe

clean:
	-rm *.mod *.o *.so *.exe *.out
