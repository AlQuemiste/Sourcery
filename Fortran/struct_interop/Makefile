name = struct_interop

src := ${name}.f90
libname := lib${name}.so

testsrc := test_${name}.c
testexename := test_${name}

builddir := "/tmp/"

all: ${libname} ${testexename}

${libname}: ${src}
	@echo "* build '$@' (build-dir = '${builddir}')"
	gfortran -std=f2008 -Wall -pedantic -O3 -cpp -fPIC -shared -J${builddir} -o $@ $<

${testexename}: ${testsrc}
	@echo "* build '$@' (build-dir = '${builddir}')"
	gcc -std=c99 -Wall -pedantic -O3 -J${builddir} -L. -o $@ $< -l:${libname} -Wl,--disable-new-dtags,-rpath,\$$ORIGIN

test: ${testexename}
	@echo "* test ${testexename}"
	./${testexename}

clean:
	-rm ${libname} ${testexename}
