echo compile foo.c
gcc -c -Wall -Werror -fPIC foo.c

echo make shared library
gcc -shared -Wl,-soname,libfoo.so -o libfoo.so foo.o
#gcc -Wall -L‍/home/utopia/Documents/shlib/ -lfoo -o test main.c 

echo make exe
gcc -L./  -Wl,-rpath='$ORIGIN' -lfoo -Wall -o test main.c

# SONAME is needed at run time. At compilation time, only a linker name or real name is needed 
