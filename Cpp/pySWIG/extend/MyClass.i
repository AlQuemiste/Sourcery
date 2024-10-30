// module name
%module MyClass

%{
// NOTE: "Python.h" is included automatically by SWIG
#include "MyClass.h"
#include <iostream>
#include <stdexcept>
%}


// print the name of Python function
%ignore _pyFuncName;
%ignore _pyCallFunc;
%ignore SWIG_FooWrapper;

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

//========================================

class SWIG_FooWrapper: public Foo {

public:

    SWIG_FooWrapper() {}

    SWIG_FooWrapper(PyObject* foo, const int i_):
        Foo{i_},
        m_foo{foo}
    {
        if (!m_isFoo(foo)) {
            throw std::runtime_error("SWIG: The given Python object "
                "is not of proper type.");
        }

        std::cout << "SWIG: SWIG_FooWrapper<" << this << "> "
            "initialized with i = " << i_ << std::endl;
    }

    int calc(const int j) override
    {
        std::cout << "SWIG: SWIG_FooWrapper<" << this << ">.calc "
            "with j = " << j << std::endl;

        return m_pyCalc(j);
    }

private:

    PyObject* m_foo = nullptr;

    const char* m_method_name = "calc";

    int m_isFoo(PyObject* pObj)
    {
        return PyObject_HasAttrString(m_foo, m_method_name);
    }

    int m_pyCalc(const int i_) {
        // call a function which takes a single Python integer as input

        std::printf("_pyCalc: calling '%s' method of Foo...\n",
                    m_method_name);

        PyObject* pMethod = PyObject_GetAttrString(m_foo, m_method_name);

        if (!PyCallable_Check(pMethod)) {
            Py_DECREF(pMethod);
            PyErr_SetString(PyExc_TypeError,
              "_pyCalc: first argument must be a Python callable");
            return 0;
        }

        // prepare the call argument
        PyObject* arg1 = PyLong_FromLong(i_);
        PyObject* pResult = PyObject_CallFunctionObjArgs(pMethod, arg1, NULL);
        Py_DECREF(pMethod);
        Py_DECREF(arg1);

        if (!pResult) {
            PyErr_SetString(PyExc_RuntimeError,
              "_pyCalc: calling Python function failed.");
            return 0;
        }

        PyTypeObject* pType = Py_TYPE(pResult); // borrowed reference
        const char* pType_name = pType->tp_name;
        std::printf("_pyCalc: calling Python function done: "
                    "returned <%s>\n", pType_name);

        int val = 0;

        if (PyLong_Check(pResult)) {
            val = static_cast<int>(PyLong_AsLong(pResult));
        } else if (PyFloat_Check(pResult)) {
            val = static_cast<int>(PyFloat_AsDouble(pResult));
        } else {
            Py_DECREF(pResult);
            PyErr_SetString(PyExc_RuntimeError,
              "_pyCalc: calling Python returned no integer or float failed.");
            return val;
        }

        Py_DECREF(pResult);

        return val;
    }

};

%};

%include "MyClass.h";

// extend C++ class
%extend MyClass {

// new initializer
// see <https://stackoverflow.com/a/33585437>
MyClass(const int i_, const int j_) {
    std::cout << "MyClass [SWIG] init with "
              << "i = " << i_ << ", j = " << j_ << std::endl;

    MyClass* new_obj = new MyClass();
    new_obj->i = i_ + j_;
    return new_obj;
}

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

int calcFoo(PyObject* foo, const int i_) {
    std::cout << ">> MyClass.calcFoo(SWIG)..." << std::endl;
    SWIG_FooWrapper _foo_wrapped{foo, i_};
    return $self->calcFoo(_foo_wrapped, i_);
}

};

// extend the Python class
%pythoncode %{
class MyClass(MyClass):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs);
        self._wrapped = []

    def __repr__(self):
        return "Python MyClass<%s>: i = %d" % (id(self), self.i)
%};
