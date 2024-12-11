#include <Python.h>
#include <iostream>


int callPyFunction(void* pyFunc, const double x, const double y, const long int b, double& result)
{
    PyObject* pFunc = static_cast<PyObject*>(pyFunc);
    // call a Python function with signature:
    //   (x:float, y:float, b:int) -> float

    // prepare function arguments
    PyObject* pArgs = PyTuple_New(3);
    PyTuple_SetItem(pArgs, 0, PyFloat_FromDouble(x));
    PyTuple_SetItem(pArgs, 1, PyFloat_FromDouble(y));
    PyTuple_SetItem(pArgs, 2, PyLong_FromLong(b));

    // call the Python function with arguments
    PyObject* pResult = PyObject_CallObject(pFunc, pArgs);
    Py_DECREF(pArgs);

    /* calling the function via a mixture of arguments and keyword arguments:

    PyObject* pArgs = PyTuple_New(1);  // a positional arg
    PyObject* pKwargs = PyDict_New();  // dict of keyword args

    PyTuple_SetItem(pArgs, 0, PyFloat_FromDouble(x));
    PyDict_SetItemString(pKwargs, "y", PyFloat_FromDouble(y));
    PyDict_SetItemString(pKwargs, "b", PyLong_FromLong(b));

    // call the Python function with mixed arguments
    PyObject* pResult = PyObject_Call(pFunc, pArgs, pKwargs);

    */

    if (!pResult)
        return -1;

    result = PyFloat_AsDouble(pResult);
    Py_DECREF(pResult);

    return 0;
}

int runFunction(const char py_func_name[], const char py_module_name[],
                const double x, const double y, const long int b,
                double& result)
{
    // initialize Python interpreter
    Py_Initialize();

    // import the Python module containing the function
    PyObject* pModule = PyImport_ImportModule(py_module_name);
    if (!pModule) {
        std::cerr << "Failed to load the Python module '"
                  << py_module_name << "'\n";
        return 1;
    }

    // Python function: (x:float, y:float, b:int) -> float
    PyObject* pFunc = PyObject_GetAttrString(pModule, py_func_name);
    if (!pFunc || !PyCallable_Check(pFunc)) {
        std::cerr << "Failed to load the Python function'"
                  << py_func_name << "'\n";
        return 1;
    }

    const int status = callPyFunction(pFunc, x, y, b, result);

    // clean up
    Py_DECREF(pModule);

    // finalize Python interpreter
    Py_Finalize();

    return status;
}
