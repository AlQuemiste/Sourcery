// module name
%module MyClass

%{
// NOTE: "Python.h" is included automatically by SWIG
#include "MyClass.h"
%}


// print the name of Python function
%ignore _pyFuncName;

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
};

// extend the Python class
%pythoncode %{
    class MyClass(MyClass):
        def __init__(self):
            super().__init__();

        def __repr__(self):
             return "Python MyClass<%s>: i = %d" % (id(self), self.i)
%};
