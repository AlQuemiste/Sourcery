from setuptools import setup, Extension
from Cython.Build import cythonize

setup(
    ext_modules = cythonize(
        [Extension("fortmath", ["fort.pyx"],
         include_dirs=[], # -I<include-dir>
         libraries=[],  # = -l<libname>
         
         library_dirs=["."],  # = -L<libdir>
         extra_compile_args=["-O3"],
         extra_link_args=["-DSOME_DEFINE_OPT",
                          "-Wl,-rpath,$ORIGIN:$ORIGIN/ext"],
         extra_objects=["./meshlib.so"],
         py_limited_api=True #  flag to use Python’s limited API
         )
         ]
)
)


# https://cython.readthedocs.io/en/latest/src/userguide/source_files_and_compilation.html
# https://setuptools.pypa.io/en/latest/userguide/ext_modules.html
# python3 setup.py build_ext --inplace
