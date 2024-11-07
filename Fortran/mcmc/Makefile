FORT := gfortran
WARN_FLAGS := -Wall -pedantic
OPT_FLAGS := -O3 -march=native
F_COMPILE_FLAGS := -std=f2008 -cpp ${WARN_FLAGS}
F_SHARED_FLAGS := -shared -fPIC
F_SRC := ising_mcmc.f90
LIBNAME := isingmc
LIBNAME_FULL := lib${LIBNAME}.so

lib: ${F_SRC}
	@echo "Make library '${LIBNAME_FULL}'..."
	${FORT} ${F_COMPILE_FLAGS} ${OPT_FLAGS} ${F_SHARED_FLAGS} -o ${LIBNAME_FULL} ${F_SRC}

test:
	python3 libisingmc.py

clean:
	-rm *.mod *.so
