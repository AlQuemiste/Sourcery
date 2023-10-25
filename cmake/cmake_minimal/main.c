#include <stdio.h>
#include <dlfcn.h>

#include "foo.h"
#include "qux.h"

int result(void)
{
    // load the shared library
    void *handle;
    handle = dlopen("libqux.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        return -1;
    }

    // get a pointer to the function
    int (*function)(void);
    char *error;
    function = dlsym(handle, "qux");
    error = dlerror();
    if (error != NULL) {
        fprintf(stderr, "%s\n", error);
        return 1;
    }

    // call the function
    const int res = function();

    // unload the shared library
    if (handle)
        dlclose(handle);

    return res;
}

int main(void)
{
    puts("This is a shared library test...");
    int number;
    printf("> Enter an integer: ");
    scanf("%d", &number);
    printf("Selection: %d\n", number);
    if (number == 1) {
        foo();
    } else {
        int res = result();
        printf("> result: %d\n", res);
    }

    puts("--END--");
    return 0;
}
