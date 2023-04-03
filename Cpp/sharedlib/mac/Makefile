.PHONY: sharedlibs clean

# see: man ld
L_STRIP := -Wl,-dead_strip -Wl,-dead_strip_dylibs
CC := clang $(L_STRIP)
CC_SHARED := $(CC) -dynamiclib

exe : main.c sharedlibs
	$(CC) -o exe main.c -L./libs  -lmy -lother -Wl,-rpath,@loader_path -Wl,-rpath,@loader_path/lib -Wl,-dead_strip -Wl,-dead_strip_dylibs  # -Wl,-why_load

sharedlibs : mylib.c mylib.h myotherlib.c myotherlib.h
	mkdir -p libs
	$(CC_SHARED) -o libs/libmy.dylib -install_name @rpath/libmy.dylib mylib.c
	$(CC_SHARED) -o libs/libother.dylib -install_name @rpath/libother.dylib myotherlib.c

clean:
	-rm *.o *.out *.so *.dylib exe
	-rm -r libs
