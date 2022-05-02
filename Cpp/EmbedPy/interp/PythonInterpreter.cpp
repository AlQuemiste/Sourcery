#include "PythonInterpreter.h"

#define PY_SSIZE_T_CLEAN
#include <Python.h>

//#define PY_ARRAY_UNIQUE_SYMBOL BORNAGAIN_PYTHONAPI_ARRAY
//#define NO_IMPORT_ARRAY
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>
//========================================

#include <cstddef>  // NULL
#include <cstring>  // memcpy
#include <string>
#include <sstream>
#include <vector>

#include <iostream>  // DEBUG
//========================================


PythonInterpreter::PythonInterpreter()
{
    std::cout << "AN>> PythonInterpreter instantiated" << std::endl;
}

PythonInterpreter::~PythonInterpreter()
{
    finalize();
    std::cout << "AN>> PythonInterpreter destroyed" << std::endl;
}

// Python stable ABI
Status PythonInterpreter::initialize()
{
    std::stringstream msg_ss;

    if (!Py_IsInitialized()) {
        Py_Initialize();
        msg_ss << "PythonInterpreter: Initialized Python - "
               << "version: " << Py_GetVersion();
        return {Status::Type::Info, msg_ss.str()};

    }

    // if Python already initialized
    msg_ss << "PythonInterpreter: Python already initialized - "
           << "version: " << Py_GetVersion();
    return {Status::Type::Warning, msg_ss.str()};
}

// Python stable ABI
bool PythonInterpreter::isInitialized()
{
    return static_cast<bool>(Py_IsInitialized());
}


// Python stable ABI
Status PythonInterpreter::finalize()
{
    // undo all initializations made by Py_Initialize() and subsequent use
    // of Python/C API functions, and destroy all sub-interpreters.
    // This is a no-op when called for a second time.
    Py_Finalize();

    std::stringstream msg_ss;
    msg_ss << "PythonInterpreter: Finalized Python - "
           << "version: " << Py_GetVersion();
    return {Status::Type::Info, msg_ss.str()};
}


// Python stable ABI
Status PythonInterpreter::checkError()
{
    if (PyErr_Occurred()) {
        // print a standard traceback to sys.stderr and clear the error indicator
        std::cerr << "---PythonInterpreter: Error in Python interpreter:\n";
        PyErr_Print();
        std::cerr << "\n---\n";
        return {Status::Type::Error};
    }

    return {Status::Type::Info, "No Python errors occured"};
}


// Python stable ABI
void PythonInterpreter::addPythonPath(const std::string& path)
{
    if (!path.empty()) {
        // add path to `PYTHONPATH`
        PyObject* sysPath = PySys_GetObject((char*)"path");
        PyList_Append(sysPath, PyUnicode_FromString(path.c_str()));
    }
}

// Python stable ABI
int PythonInterpreter::setPythonPath(const std::string& path)
{
    // returns 0 on success, -1 on error
    const int result = PySys_SetObject(
        (char*)"path", PyUnicode_FromString(path.c_str()));
    return result;
}

// Python stable ABI
PyObjectPtr PythonInterpreter::import(
    const std::string& pymodule_name, const std::string& path)
{
    addPythonPath(path);

    // import the module
    PyObject* pymodule = PyImport_ImportModule(pymodule_name.c_str());
    if (!pymodule || !PyModule_Check(pymodule)) {
        checkError();
        std::stringstream msg_ss;
        msg_ss << "PythonInterpreter: Cannot load Python module "
               << "'" << pymodule_name << "' "
               << "(given path = '" << path << "')";
        return PyObjectPtr(Status(Status::Type::Error, msg_ss.str()));
    }

    // returns a _new_ reference; ie. caller is responsible for the ref-count
    return {pymodule};
}


// Python stable ABI
void PythonInterpreter::DecRef(PyObject* py_object)
{
    Py_XDECREF(py_object);
}

//----------------------------------------

Status PythonInterpreter::Numpy::initialize() {

    // initialize Python C API, if needed
    Status py_status{PythonInterpreter::initialize()};
    if(!py_status)
        return py_status;

    const int res {_init_numpy()};

    switch(res)
    {
    case 1:
        return {Status::Type::Info, "PythonInterpreter: Initialized Numpy"};
    case 2:
        return {Status::Type::Warning, "PythonInterpreter: Numpy already initialized"};
    default:
        return {Status::Type::Error, "PythonInterpreter: Cannot initialize Numpy"};
    }
}


int PythonInterpreter::Numpy::_init_numpy()
{
    if(!PyArray_API)
    {
        // import Numpy Array API (should be called only once)
        // import_array returns `NULL` on failure.
        import_array();
        return 1; // Numpy Array API loaded successfully
    }

    return 2;  // Numpy Array API is already loaded
}


PyObjectPtr PythonInterpreter::Numpy::call_pyfunction_npy_array(
    PyObjectPtr& py_module, const std::string& fn_name, PyObjectPtr& npy_array)
{
    // Call a Python function (from a given module) which takes a
    // single Numpy array as input and returns a Numpy array.

    if (!py_module.valid()) {
        return {Status(Status::Type::Error,
                       "PythonInterpreter: Cannot call a function "
                       "from an invalid Python module")};
    }

    PyObject* pFunc = PyObject_GetAttrString(py_module.ptr, fn_name.c_str());
    if (!pFunc || !PyCallable_Check(pFunc)) {
        PythonInterpreter::checkError();
        std::stringstream msg_ss;
        msg_ss << "PythonInterpreter: The function '" << fn_name << "' "
               << "is not callable";
        return {Status(Status::Type::Error, msg_ss.str())};
    }

    PyObject* pReturn = PyObject_CallFunctionObjArgs(pFunc, npy_array.ptr,
                                                     NULL);
    if (!PyArray_Check(pReturn)) {
        PythonInterpreter::checkError();
        Py_DecRef(pFunc);
        std::stringstream msg_ss;
        msg_ss << "PythonInterpreter: Invalid return value from calling "
            "the function '" << fn_name << "' (expected a Numpy array)";
        return {Status(Status::Type::Error, msg_ss.str())};
    }

    // returns a _new_ reference; ie. caller is responsible for the ref-count
    return {pReturn};
}


PyObjectPtr PythonInterpreter::Fabio::import()
{
    return {PythonInterpreter::import("fabio")};
}


PyObjectPtr PythonInterpreter::Fabio::open(const std::string& filename,
                                           PyObjectPtr& fabio_module)
{
    // load an image via calling `fabio.load` function which takes a
    // filename (Python string) and returns a Numpy array.

    if (!fabio_module.valid() || !PyModule_Check(fabio_module.ptr)) {
        return {Status(Status::Type::Error,
                       "PythonInterpreter.fabio: Invalid Python module "
                       "(expected 'fabio' module)")};
    }

    PyObject* pFunc = PyObject_GetAttrString(fabio_module.ptr, (char*)"open");
    if (!pFunc || !PyCallable_Check(pFunc)) {
        PythonInterpreter::checkError();
        return {Status(Status::Type::Error,
                       "PythonInterpreter.fabio: The function 'fabio.open' is not callable")};
    }

    // convert the filename to a Python unicode string
    PyObject* pFilename = PyUnicode_FromString(filename.c_str());
    if (!pFilename) {
        PythonInterpreter::checkError();
        std::stringstream msg_ss;
        msg_ss << "PythonInterpreter.fabio: Filename '" << filename << "' "
               << "cannot be converted to Python unicode string";
        return {Status(Status::Type::Error, msg_ss.str())};
    }

    // get the result of the call `fabio.open(<filename>)`
    PyObject* pResult_open = PyObject_CallOneArg(pFunc, pFilename);
    Py_DecRef(pFunc);
    if (!pResult_open) {
        PythonInterpreter::checkError();
        std::stringstream msg_ss;
        msg_ss << "PythonInterpreter.fabio: Invalid return value from "
               << "calling the function 'fabio.open(\"" << filename << "\")'";
        return {Status(Status::Type::Error, msg_ss.str())};
    }

    // get `result.data` (must be a Numpy array)
    PyObject* npyArray_ptr = PyObject_GetAttrString(pResult_open, (char*)"data");
    Py_DecRef(pResult_open);
    if (!npyArray_ptr || !PyArray_Check(npyArray_ptr)) {
        PythonInterpreter::checkError();
        std::stringstream msg_ss;
        msg_ss << "PythonInterpreter.fabio: Invalid return value from "
               << "calling the function 'fabio.open(\"" << filename << "\")' "
               << "(expected a Numpy array)";
        return {Status(Status::Type::Error, msg_ss.str())};
    }

    // returns a _new_ reference; ie. caller is responsible for the ref-count
    return {npyArray_ptr};
}


double* PythonInterpreter::Numpy::NpyArray_to_C(PyObjectPtr& npy_array)
{
    if(!npy_array.valid() || !PyArray_Check(npy_array.ptr)) {
        throw std::runtime_error(
            "PythonInterpreter: Cannot convert an invalid Numpy array to a C-Array");
    }

    PyArrayObject* npArray_ptr {reinterpret_cast<PyArrayObject*>(npy_array.ptr)};
    PyArray_Descr* npArray_descr = PyArray_DESCR(npArray_ptr);
    const int npArray_type = PyArray_TYPE(npArray_ptr);
    const int npArray_ndim = PyArray_NDIM(npArray_ptr);
    npy_intp* npArray_dims = PyArray_DIMS(npArray_ptr);
    const int npArray_flags = PyArray_FLAGS(npArray_ptr);

    // DEBUG
    // type checking
    if(PyDataType_ISUNSIGNED(npArray_descr))
        printf("AN>> Array datatype = uint\n");
    else if(PyDataType_ISSIGNED(npArray_descr))
        printf("AN>> Array datatype = int\n");
    else if(PyDataType_ISFLOAT(npArray_descr))
        printf("AN>> Array datatype = float\n");
    else
        printf("AN>> Array datatype = ?\n");

    if(npArray_type == NPY_FLOAT)
        printf("AN>> Array type (%d) = f32\n", npArray_type);

    if(PyArray_IS_C_CONTIGUOUS(npArray_ptr))
        printf("AN>> Array is C-contiguous (row-major)\n");

    if(PyArray_IS_F_CONTIGUOUS(npArray_ptr))
        printf("AN>> Array is Fortran-contiguous (column-major)\n");

    if(PyArray_ISBEHAVED_RO(npArray_ptr) || PyArray_ISBEHAVED(npArray_ptr))
        printf("AN>> Array is well-behaved\n");

    if(npArray_flags & NPY_ARRAY_OWNDATA)
        printf("AN>> Array owns its data\n");
    else
        printf("AN>> Array does not own its data\n");

    for(int i_dim = 0; i_dim < npArray_ndim; ++i_dim)
        printf("AN>> Array-dim{%d} = %ld\n", i_dim, npArray_dims[i_dim]);

    // END DEBUG

    double* npArray_data_ptr {reinterpret_cast<double*>(PyArray_DATA(npArray_ptr))};
    if(!npArray_data_ptr) {
        PythonInterpreter::checkError();
        throw std::runtime_error("PythonInterpreter: Cannot convert an invalid Numpy array to a C-Array (invalid data pointer)");
    }

    // TODO: Problem with the reference to numpy array!!!
    // returns a _new_ reference; ie. caller is responsible for the Python ref-count
    return npArray_data_ptr;
}

double* npyArray_to_CArray_WIP(PyObjectPtr& npy_array)
{
    if(!npy_array.valid() || !PyArray_Check(npy_array.ptr)) {
        throw std::runtime_error(
            "PythonInterpreter: Cannot convert an invalid Numpy array to a C-Array");
    }

    PyArrayObject* npArray_ptr {reinterpret_cast<PyArrayObject*>(npy_array.ptr)};
    PyArray_Descr* npArray_descr = PyArray_DESCR(npArray_ptr);
    const int npArray_type = PyArray_TYPE(npArray_ptr);
    const int npArray_ndim = PyArray_NDIM(npArray_ptr);
    npy_intp* npArray_dims = PyArray_DIMS(npArray_ptr);
    const int npArray_flags = PyArray_FLAGS(npArray_ptr);

    // DEBUG
    // type checking
    if(PyDataType_ISUNSIGNED(npArray_descr))
        printf("AN>> Array datatype = uint\n");
    else if(PyDataType_ISSIGNED(npArray_descr))
        printf("AN>> Array datatype = int\n");
    else if(PyDataType_ISFLOAT(npArray_descr))
        printf("AN>> Array datatype = float\n");
    else
        printf("AN>> Array datatype = ?\n");

    if(npArray_type == NPY_FLOAT)
        printf("AN>> Array type (%d) = f32\n", npArray_type);

    if(PyArray_IS_C_CONTIGUOUS(npArray_ptr))
        printf("AN>> Array is C-contiguous (row-major)\n");

    if(PyArray_IS_F_CONTIGUOUS(npArray_ptr))
        printf("AN>> Array is Fortran-contiguous (column-major)\n");

    if(PyArray_ISBEHAVED_RO(npArray_ptr) || PyArray_ISBEHAVED(npArray_ptr))
        printf("AN>> Array is well-behaved\n");

    if(npArray_flags & NPY_ARRAY_OWNDATA)
        printf("AN>> Array owns its data\n");
    else
        printf("AN>> Array does not own its data\n");

    for(int i_dim = 0; i_dim < npArray_ndim; ++i_dim)
        printf("AN>> Array-dim{%d} = %ld\n", i_dim, npArray_dims[i_dim]);

    // END DEBUG

    // TODO: Verify array is 2d

    // void *PyArray_GETPTR2(PyArrayObject *obj, npy_intp i, npy_intp j)
    // Quick, inline access to the element at the given coordinates in the ndarray, obj, which must have 2 dimensions (this is not checked). The corresponding i, j,  coordinates can be any integer but will be interpreted as npy_intp. You may want to typecast the returned pointer to the data type of the ndarray.

    // element-by-element copy of the Numpy array data (always work)
    const std::size_t vsize = static_cast<std::size_t>(PyArray_SIZE(npArray_ptr));
    std::vector<double> out1(vsize);
    for (std::size_t i = 0; i < vsize; i++) {
        out1[i] = *reinterpret_cast<double*>(PyArray_GETPTR1(npArray_ptr, i));
    }

    // using bulk memcpy-like operation, but keep in mind that NumPy ndarrays may be mis-aligned for the data type, have non-native byte order, or other subtle attributes that make such copies less than desirable.

    std::vector<double> out2(vsize);
    std::memcpy(out2.data(), PyArray_DATA(npArray_ptr),
                sizeof(double) * vsize);

    // using either approach, out now contains a copy of the ndarray's data

    double* npArray_data_ptr {reinterpret_cast<double*>(PyArray_DATA(npArray_ptr))};
    if(!npArray_data_ptr) {
        // checkError();
        throw std::runtime_error("PythonInterpreter: Cannot convert an invalid Numpy array to a C-Array (invalid data pointer)");
    }

    // TODO: Problem with the reference to numpy array!!!
    // returns a _new_ reference; ie. caller is responsible for the Python ref-count
    return npArray_data_ptr;
}


PyObjectPtr PythonInterpreter::Numpy::CArray_to_Npy(double* c_array, int n_dims, long int* dims)
{
    // calculate the total size of the array
    int size = 1;
    for(int i_d = 0; i_d < n_dims; ++i_d)
        if(dims[i_d] != 0)
            size *= dims[i_d];

    npy_intp* npDims = static_cast<npy_intp*>(dims);

    // convert the contiguous C-array to Numpy array
    PyObject* npArray_ptr = PyArray_SimpleNewFromData(
        n_dims, npDims, NPY_DOUBLE, reinterpret_cast<void*>(c_array));

    if(!npArray_ptr || !PyArray_Check(npArray_ptr)) {
        PythonInterpreter::checkError();
        return {Status(Status::Type::Error,
                       "PythonInterpreter: Cannot convert the given C-Array to a Numpy array")};
    }

    // returns a _new_ reference; ie. caller is responsible for the ref-count
    return {npArray_ptr};
}
