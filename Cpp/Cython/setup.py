from setuptools import setup, Extension
from Cython.Build import cythonize

setup(
    ext_modules = cythonize(
        [Extension("foothing", ["src/foothing.pyx"],
                   include_dirs=[], # = -I<include-dir>
                   libraries=[],  # = -l<libname>

                   library_dirs=["."],  # = -L<libdir>
                   extra_compile_args=["-O3"],
                   extra_link_args=["-DSOME_DEFINE_OPT",
                          "-Wl,-rpath,$ORIGIN:$ORIGIN/ext"],
                   extra_objects=["libfoo.so"],
                   language="c++",
                   py_limited_api=True #  flag to use Python’s limited API
                   )
         ],
        build_dir="buildcy",
        nthreads=8,
        language_level=3
    ),
    script_args=['build_ext'],
    options={'build_ext': {'inplace':True}}
)
