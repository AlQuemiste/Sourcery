// https://medium.com/swlh/make-python-hundreds-of-times-faster-with-a-c-extension-9d0a5180063e
// https://github.com/jweinst1/PyCExtension

// must include Python.h before any standard headers are included
#define PY_SSIZE_T_CLEAN
#include <Python.h>

static PyObject* print_message(PyObject* self, PyObject* args)
{
    const char* str_arg;
    if(!PyArg_ParseTuple(args, "s", &str_arg)) {
        puts("Error: Could not parse the python arg!");
        Py_RETURN_NONE;
    }

    printf("Message>> %s\n", str_arg);
    // This can also be done with Py_RETURN_NONE
    Py_RETURN_NONE;
}


static PyMethodDef myMethods[] =
{
  {"print_message", print_message, METH_VARARGS, "Prints a string"},
  //----------------------------------
  {NULL, NULL, 0, NULL} /* Sentinel */
};

// Our Module Definition struct

static struct PyModuleDef myModule = {
    PyModuleDef_HEAD_INIT,
    "DemoPackage", /* name of module */
    "A demo module for python c extensions", /* module documentation, may be NULL */
    -1, /* size of per-interpreter state of the module,
           or -1 if the module keeps state in global variables. */
    myMethods
};


// Initializes our module using our above struct
/* PyMODINIT_FUNC declares the function as PyObject* return type,
   declares any special linkage declarations required by the platform,
   and for C++ declares the function as extern "C".
*/
PyMODINIT_FUNC PyInit_DemoPackage(void)
{
    return PyModule_Create(&myModule);
}
