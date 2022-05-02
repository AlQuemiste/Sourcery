// ref: <https://codereview.stackexchange.com/q/92266>

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include "Python.h"
#include "numpy/arrayobject.h"
#include <iostream>

int main(int argc, char* argv[])
{
    int result = EXIT_FAILURE;

    Py_SetProgramName(argv[0]);
    Py_Initialize();
    import_array();

    // Build the 2D array in C++
    const int SIZE = 10;
    npy_intp dims[2]{SIZE, SIZE};
    const int ND = 2;
    long double(*c_arr)[SIZE]{ new long double[SIZE][SIZE] };
    if (!c_arr) {
        std::cerr << "Out of memory." << std::endl;
        goto fail_c_array;
    }
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            c_arr[i][j] = i * SIZE + j;

    // Convert it to a NumPy array.
    PyObject *pArray = PyArray_SimpleNewFromData(
        ND, dims, NPY_LONGDOUBLE, reinterpret_cast<void*>(c_arr));
    if (!pArray)
        goto fail_np_array;
    PyArrayObject *np_arr = reinterpret_cast<PyArrayObject*>(pArray);

    // import mymodule.array_tutorial
    const char *module_name = "mymodule";
    PyObject *pName = PyUnicode_FromString(module_name);
    if (!pName)
        goto fail_name;
    PyObject *pModule = PyImport_Import(pName);
    Py_DECREF(pName);
    if (!pModule)
        goto fail_import;
    const char *func_name = "array_tutorial";
    PyObject *pFunc = PyObject_GetAttrString(pModule, func_name);
    if (!pFunc)
        goto fail_getattr;
    if (!PyCallable_Check(pFunc)){
        std::cerr << module_name << "." << func_name
                  << " is not callable." << std::endl;
        goto fail_callable;
    }

    // np_ret = mymodule.array_tutorial(np_arr)
    PyObject *pReturn = PyObject_CallFunctionObjArgs(pFunc, pArray, NULL);
    if (!pReturn)
        goto fail_call;
    if (!PyArray_Check(pReturn)) {
        std::cerr << module_name << "." << func_name
                  << " did not return an array." << std::endl;
        goto fail_array_check;
    }
    PyArrayObject *np_ret = reinterpret_cast<PyArrayObject*>(pReturn);
    if (PyArray_NDIM(np_ret) != ND - 1) {
        std::cerr << module_name << "." << func_name
                  << " returned array with wrong dimension." << std::endl;
        goto fail_ndim;
    }

    // Convert back to C++ array and print.
    int len = PyArray_SHAPE(np_ret)[0];
    long double* c_out = reinterpret_cast<long double*>(PyArray_DATA(np_ret));
    std::cout << "Printing output array" << std::endl;
    for (int i = 0; i < len; i++)
        std::cout << c_out[i] << ' ';
    std::cout << std::endl;
    result = EXIT_SUCCESS;

fail_ndim:
fail_array_check:
    Py_DECREF(pReturn);
fail_call:
fail_callable:
    Py_DECREF(pFunc);
fail_getattr:
    Py_DECREF(pModule);
fail_import:
fail_name:
    Py_DECREF(pArray);
fail_np_array:
    delete[] c_arr;
fail_c_array:
    if (PyErr_Check())
        PyErr_Print();
    Py_Finalize();
    return result;
}
