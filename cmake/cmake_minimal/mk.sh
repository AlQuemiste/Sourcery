CC=gcc

build_dir=./bld/
foolibname=libfoo.so
quxlibname=libqux.so
exename=test

C_FLAGS=$C_FLAGS
mkdir -p $build_dir

echo "* compile..."
$CC -c $C_FLAGS -o $build_dir/foo.o foo.c
$CC -c $C_FLAGS -o $build_dir/qux.o qux.c
$CC -c $C_FLAGS -o $build_dir/$exename.o main.c

echo "* make shared library $foolibname..."
$CC -shared -fPIC -Wl,-soname,$foolibname -o $build_dir/$foolibname  $build_dir/foo.o

echo "* make shared library $quxlibname..."
$CC -shared -fPIC -Wl,-soname,$quxlibname -o $build_dir/$quxlibname  $build_dir/qux.o

echo "* make exe..."
$CC  -Wl,-rpath='$ORIGIN' -o $build_dir/$exename $build_dir/$exename.o -L$build_dir -lfoo

# SONAME is needed at run time. At compilation time, only a linker name or real name is needed
