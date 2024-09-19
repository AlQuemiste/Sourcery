#include <Python.h>
#include <iostream>

int callPyFunction(PyObject* pFunc, const double x, const double y, const long int b, double& result)
{
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


int main() {
    // initialize the Python interpreter
    Py_Initialize();

    // import the Python module containing the function
    const char py_module_name[] = "my_module";
    PyObject* pModule = PyImport_ImportModule(py_module_name);
    if (!pModule) {
        std::cerr << "Failed to load the Python module '"
        << py_module_name << "'\n";
        return 1;
    }

    // Python function: (x:float, y:float, b:int) -> float
    const char py_func_name[] = "real_fn";
    PyObject* pFunc = PyObject_GetAttrString(pModule, py_func_name);
    if (!pFunc || !PyCallable_Check(pFunc)) {
        std::cerr << "Failed to load the Python function'"
        << py_func_name << "'\n";
        return 1;
    }

    // Prepare arguments for the function call
    const double x = 3.14, y = 2.0;
    const long int b = -42;
    double result;

    std::cout << "C++: call Python function '" << py_func_name << "' with "
              << "x = " << x << ", y = " << y << ", b = " << b << std::endl;

    const int status = callPyFunction(pFunc, x, y, b, result);

    if (status != 0) {
        std::cerr << "C++: Python function '" << py_func_name << "' failed\n";
    } else {
        std::cout << "C++: " << py_func_name << " result: " << result << std::endl;
    }

    // clean up
    Py_DECREF(pModule);

    // finalize the Python interpreter
    Py_Finalize();

    return 0;
}
