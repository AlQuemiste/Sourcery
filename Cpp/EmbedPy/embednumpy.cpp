#define PY_SSIZE_T_CLEAN
#include <Python.h>

//#define PY_ARRAY_UNIQUE_SYMBOL BORNAGAIN_PYTHONAPI_ARRAY
//#define NO_IMPORT_ARRAY
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>

#include <stdlib.h> // EXIT_FAILURE, EXIT_SUCCESS
#include <stdio.h>
#include <assert.h>
#include <iostream>

void initialize_python(const bool verbose=false)
{
    if (!Py_IsInitialized()) {
        // return true (nonzero) when the Python interpreter has been initialized, false (zero) if not
        Py_Initialize();
        if(verbose) {
            printf("* Python initialized: version: %s (platform: %s)\n",
                   Py_GetVersion(), Py_GetPlatform());
        }

    } else if(verbose) {
        printf("* Python already initialized: version: %s (platform: %s)\n",
               Py_GetVersion(), Py_GetPlatform());
    }

}

void finalize_python(const bool verbose=false)
{
    // Undo all initializations made by Py_Initialize() and subsequent use of Python/C API functions, and destroy all sub-interpreters. This is a no-op when called for a second time.
    Py_Finalize();
    if(verbose) {
        printf("* Python finalized: version: %s (platform: %s)\n",
               Py_GetVersion(), Py_GetPlatform());
    }
}

int check_occured_err(void) {
    if (PyErr_Occurred()) {
        PyErr_Print();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int init_numpy(const bool verbose=false) {
    initialize_python(verbose);

    if(PyArray_API == NULL)
    {
        import_array();
        if(verbose) {
            puts("* Numpy initialized.");
        }
    } else {
        if(verbose) {
            puts("* Numpy already initialized.");
        }
    }
    assert(PyArray_API);
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
    initialize_python(true);
    // Initialise Numpy
    init_numpy(true);

    check_occured_err();
    // Py_Initialize();
    // // Initialise Numpy
    // import_array();

    // if (PyErr_Occurred()) {
    //     printf("Failed to import numpy Python module(s).\n");
    //     return EXIT_FAILURE;
    // }

    //========================================

    /*
    const int nd = 2;
    npy_intp dims[] = {5, 3};
    int typenum = NPY_FLOAT;
    PyObject* myarr = PyArray_SimpleNew(nd, dims, typenum);
    printf("data = %x", myarr->data);
    */

    int result = EXIT_FAILURE;

    printf("NPY_VERSION: %#010X , "
           "NPY_FEATURE_VERSION: %#010X\n",
           NPY_VERSION, NPY_FEATURE_VERSION);
    /* Py_SetProgramName(argv[0]); */

    // declare all variables
    // NOTE: C++ standard:
    // It is possible to transfer into a block, but not in a way that bypasses declarations with initialization. A program that jumps from a point where a local variable with automatic storage duration is not in scope to a point where it is in scope is ill-formed unless the variable has POD type (3.9) and is declared without an initializer.
    // This is needed to avoid compiler error "jump to label crosses initialization...".
    PyObject *pArray, *pName, *pModule, *pFunc, *pReturn;
    PyArrayObject *np_arr, *np_ret;
    const char module_name[] = "mymodule";
    const char func_name[] = "array_tutorial";

    // Build the 2D array
    const int SIZE = 10;
    int ND = 2;
    npy_intp dims[2] = {SIZE, SIZE};

    long double (*c_arr)[SIZE] { new long double[SIZE][SIZE] };

    // check memory allocation
    if (!c_arr) {
        printf("Out of memory.\n");
        goto fail_c_array;
    }

    // fill the array
    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            c_arr[i][j] = i * SIZE + j;

    // convert C-array to Numpy array
    pArray = PyArray_SimpleNewFromData(
        ND, dims, NPY_LONGDOUBLE, reinterpret_cast<void*>(c_arr));

    if (!pArray)
        goto fail_np_array;

    np_arr = reinterpret_cast<PyArrayObject*>(pArray);

    // import mymodule.array_tutorial
    pName = PyUnicode_FromString(module_name);
    if (!pName)
        printf("goto fail_name\n");

    // add path of the module (currend dir) to PYTHONPATH
    PyRun_SimpleString("import sys, os;");
    PyRun_SimpleString("sys.path.append(os.getcwd())");
    pModule = PyImport_Import(pName);
    Py_XDECREF(pName);
    if (!pModule)
        printf("goto fail_import\n");

/* func_name = "array_tutorial"; */
    pFunc = PyObject_GetAttrString(pModule, func_name);
    /* if (!pFunc) */
    /*     goto fail_getattr; */
    if (!PyCallable_Check(pFunc)){
        printf("module.func is not callable\n"); // , module_name, func_name);
        /* goto fail_callable; */
    }

    // np_ret = mymodule.array_tutorial(np_arr)
    pReturn = PyObject_CallFunctionObjArgs(pFunc, pArray, NULL);
    if (!pReturn)
        printf("goto fail_call.\n");
    if (!PyArray_Check(pReturn)) {
        printf("module_name.func_name did not return an array.\n");
        printf("goto fail_array_check\n");
    }

    np_ret = reinterpret_cast<PyArrayObject*>(pReturn);
    if (PyArray_NDIM(np_ret) != ND - 1) {
        printf("module_name.func_name returned array with wrong dimension.\n");
        printf("goto fail_ndim\n");
    }


    //========================================
fail_np_array:
    delete[] c_arr;

fail_c_array:
    if (PyErr_Occurred())
        PyErr_Print();

    // Undo all initializations made by Py_Initialize() and subsequent use of Python/C API functions, and destroy all sub-interpreters. This is a no-op when called for a second time.
    // Py_Finalize();
    finalize_python(true);
    return 0;
}
