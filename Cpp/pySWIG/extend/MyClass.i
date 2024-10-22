// module name
%module MyClass

%{
// NOTE: "Python.h" is included automatically by SWIG
#include "MyClass.h"
%}


// print the name of Python function
%ignore _pyFuncName;
%ignore _pyCallFunc;

%inline %{

void _pyFuncName(void* pyFunc) {
    // get __name__ attribute of the Python object
    PyObject* pName = PyObject_GetAttrString(static_cast<PyObject*>(pyFunc), "__name__");

    if (pName && PyUnicode_Check(pName)) {
        // convert Python string to C-string
        const char* fnName = PyUnicode_AsUTF8(pName);
        printf("MyClass.configure(SWIG): Python function name: '%s'\n", fnName);
        Py_DECREF(pName);
    } else {
        Py_XDECREF(pName);
        PyErr_SetString(PyExc_TypeError,
          "MyClass.configure(SWIG): Could not retrieve the name of the Python function");
    }
}

void _pyCallFunc(PyObject* pyFunc, PyObject* arg1) {
    // call a function which takes a single Python object as input

    if (!PyCallable_Check(pyFunc)) {
        PyErr_SetString(PyExc_TypeError,
          "_pyCallFunc: first argument must be a Python callable");
    }

    std::puts("_pyCallFunc: calling Python function...");

    PyObject* pResult = PyObject_CallFunctionObjArgs(pyFunc, arg1, NULL);
    if (!pResult) {
        PyErr_SetString(PyExc_RuntimeError,
          "_pyCallFunc: calling Python function failed.");
    }

    PyTypeObject* pType = Py_TYPE(pResult); // borrowed reference
    const char* pType_name = pType->tp_name;
    std::printf("_pyCallFunc: calling Python function done: returned <%s>\n", pType_name);

    Py_DECREF(pResult);
}

%};

%include "MyClass.h";

// extend C++ class
%extend MyClass {
    void configure(PyObject* pFunc, const int i_) {
        if (PyCallable_Check(pFunc)) {
            // set the function pointer for a later call
            $self->pyGetName = _pyFuncName;

            // pass the Python callable to the original 'configure' method
            $self->configure(static_cast<void*>(pFunc), i_);
        } else {
            PyErr_SetString(PyExc_TypeError,
              "MyClass.configure(SWIG): First argument must be a Python callable");
        }
    }

    void callPyFunction(PyObject* pFunc, PyObject* info) {
        _pyCallFunc(pFunc, info);
    }
};

// extend the Python class
%pythoncode %{
    class MyClass(MyClass):
        def __init__(self):
            super().__init__();

        def __repr__(self):
             return "Python MyClass<%s>: i = %d" % (id(self), self.i)
%};
