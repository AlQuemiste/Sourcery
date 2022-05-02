#ifndef PYTHONINTRP_H
#define PYTHONINTRP_H

#include <string>
#include "PyObjectPtr.h"

// Stateless container for embedded Python interpreter
class PythonInterpreter {

public:
    PythonInterpreter();
    ~PythonInterpreter();

    //----------------------------------------

    // Python stable ABI
    static Status initialize();

    // Python stable ABI
    static bool isInitialized();

    // Python stable ABI
    static Status finalize();

    // Python stable ABI
    static Status checkError();

    // Python stable ABI
    static void addPythonPath(const std::string& path);

    // Python stable ABI
    static int setPythonPath(const std::string& path);

    // Python stable ABI
    static PyObjectPtr import(const std::string& pymodule_name,
                              const std::string& path = "");
    // Python stable ABI
    static void DecRef(PyObject* py_object);

    //-- Numpy-related functionality
    class Numpy {
    public:
        static Status initialize();

        static double* NpyArray_to_C(PyObjectPtr& npy_array);

        static PyObjectPtr CArray_to_Npy(double* c_array, int n_dims, long int* dims);

    private:
        // thin wrapper to initialize Numpy
        static int _init_numpy();

        // dummy function
        static PyObjectPtr call_pyfunction_npy_array(
        PyObjectPtr& py_module, const std::string& fn_name,
        PyObjectPtr& npy_array);

    }; // class Numpy

    //-- Fabio-related functionality
    class Fabio {
    public:

        // import 'fabio' Python module
        static PyObjectPtr import();

        // call 'fabio.open' on a given file
        static PyObjectPtr open(const std::string& filename,
                                PyObjectPtr& fabio_module);
    }; // class Fabio

};

#endif  // PYTHONINTRP_H
