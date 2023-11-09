.PHONY: all, exe, testpy

all: libarr_mean.so arr_mean.exe

arr_mean.exe: arr_mean.c libarr_mean.so
	gcc -Wall -o arr_mean.exe arr_mean.c -L. -larr_mean -Wl,-rpath,.

libarr_mean.so: arr_mean.f08
	gfortran -Wall -O3 -fPIC -shared -o libarr_mean.so -std=f2008 arr_mean.f08

test:
	@echo "* run C-executable:"
	./arr_mean.exe
	@echo "* run Python script:"
	python3 arr_mean.py

clean:
	-rm *.mod *.so *.exe *.out
