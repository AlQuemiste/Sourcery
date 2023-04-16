#include "foothing.hpp"

#include <iostream>

int main() {
    // initialize Python interpreter
    Py_Initialize();

    // TODO: set PYTHONPATH properly for the module to be found

    // import your Cython module
    PyObject* module = PyImport_ImportModule("foothing");

    if(!module)
    {
        std::cout << "ERROR: Module not found" << std::endl;
        return 0;
    }

    // call a function defined in your module
    PyObject* result = PyObject_CallMethod(module, "Foo", "(ii)", 1, 2);

    if(!result)
    {
        std::cout << "ERROR: Result is not available" << std::endl;
        return 0;
    }

    // convert result to a C integer
    int value = PyLong_AsLong(result);

    std::cout << "embedded Cython: result = " << value << std::endl;

    // cleanup
    Py_XDECREF(result);
    Py_DECREF(module);
    Py_Finalize();

    return value;
}
