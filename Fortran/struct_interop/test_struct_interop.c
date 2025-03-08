#include <stdio.h>
#include <stdlib.h>

// Define the C struct
struct MyStruct {
    int i;
    double x;
    int* int_array;
    double* dbl_array;
};

struct MyVoid {
    void* my_struct;
};

// declare the Fortran subroutine
extern void f_void(void* my_void, int n, int d1, int d2);
extern void create_my_struct(void** my_struct_ptr);
extern void free_my_struct(void** my_struct_ptr);

int main() {
    // create an instance of the struct
    struct MyVoid my_void;
    struct MyStruct my_struct;

    // initialize MyVoid instance
    puts("C: initialize MyVoid");
    my_void.my_struct = (void*) &my_struct;

    // initialize MyStruct instance
    puts("C: initialize MyStruct");

    my_struct.i = 42;
    my_struct.x = 3.14;

    const int n = 5, d1 = 3, d2 = 2;

    my_struct.int_array = (int*) malloc(n * sizeof(int));
    my_struct.dbl_array = (double*) malloc(d1 * d2 * sizeof(double));

    // fill arrays with data
    for (int i = 0; i < n; i++)
        my_struct.int_array[i] = i + 1;

    for (int i = 0; i < d1; i++) {
        for (int j = 0; j < d2; j++)
            my_struct.dbl_array[i * d2 + j] = (i + 1) * 10 + (j + 1);
    }

    // call the Fortran subroutine
    puts("C: call Fortran subroutine");
    f_void(&my_void, n, d1, d2);
    puts("C: call Fortran subroutine: done.");

    // free allocated memory
    free(my_struct.int_array);
    free(my_struct.dbl_array);

    puts("C: create MyStruct via Fortran");
    void* void_ptr;
    create_my_struct(&void_ptr);

    if (void_ptr == NULL) {
        puts("C: error: failed to create MyStruct instance");
        return 1;
    }

    struct MyStruct* my_struct_ptr = (struct MyStruct*) void_ptr;
    printf("C: MyStruct: i = %d, x = %f\n", my_struct_ptr->i, my_struct_ptr->x);
    free_my_struct(&void_ptr);
    puts("C: create MyStruct via Fortran: done.");

    puts("C: test done.");
    return 0;
}
