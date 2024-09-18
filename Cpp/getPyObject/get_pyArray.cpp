#include <Python.h>
#include <cstdint>
#include <string>
#include <iostream>

typedef std::size_t size_type;

void parseArrayDescriptor(PyObject* const pyArrayDescriptorTuple,
                          size_type& arr_size, size_type& arr_ndims, size_type*& arr_dims,
                          std::string& arr_dtype, void*& array_ptr)
{
    // reset the output variables
    arr_size = 0;
    arr_ndims = 0;
    arr_dims = nullptr;
    array_ptr = nullptr;

    if (!pyArrayDescriptorTuple)
        return;

    // parse the given tuple `pyArrayWrapper`:
    //   (void-pointer, size, nr-of-dimensions, dimensions, dtype)
    PyObject* py_void_ptr;
    PyObject* py_arr_size;
    PyObject* py_ndims;
    PyObject* py_arr_dims;
    PyObject* py_arr_dtype;

    if (!PyArg_ParseTuple(pyArrayDescriptorTuple, "OOOOO",  // borrowed reference
                          &py_void_ptr, &py_arr_size, &py_ndims, &py_arr_dims, &py_arr_dtype)) {
        PyErr_Print();
        std::cerr << "C++: Failed to parse returned values from Python array-converter" << std::endl;
        return;
    }

    if (!(py_void_ptr && py_arr_size && py_ndims && py_arr_dims && py_arr_dtype)) {
        std::cerr << "C++: invalid returned values from Python array-converter" << std::endl;
        return;
    }

    // array void-pointer and metadata
    array_ptr = PyLong_AsVoidPtr(py_void_ptr);
    arr_size = static_cast<size_type>(PyLong_AsLong(py_arr_size));
    arr_ndims = static_cast<size_type>(PyLong_AsLong(py_ndims));
    // convert Python string to std::string
    PyObject* _dtype = PyUnicode_AsEncodedString(py_arr_dtype, "UTF-8", "strict");
    arr_dtype = std::string(PyBytes_AsString(_dtype));
    Py_DECREF(_dtype);

    // verify array properties
    if (!(array_ptr && arr_size > 0 && arr_ndims > 0 && !arr_dtype.empty()))
        return;

    // store each dimension of the array
    arr_dims = new size_type[arr_ndims];
    for (size_type i_d = 0; i_d < arr_ndims; ++i_d)
        arr_dims[i_d] = static_cast<size_type>(PyLong_AsLong(PyTuple_GetItem(py_arr_dims, i_d)));
}


PyObject* makeArrayDescriptor(PyObject* const numpyArray)
{
    // import ctypes
    PyObject* pCTypesModule = PyImport_ImportModule("ctypes");
    if (pCTypesModule == NULL) {
        PyErr_Print();
        std::cerr << "Failed to import ctypes" << std::endl;
        return nullptr;
    }

    // import numpy
    PyObject* pNumpyModule = PyImport_ImportModule("numpy");
    if (pNumpyModule == NULL) {
        PyErr_Print();
        std::cerr << "Failed to import numpy" << std::endl;
        Py_DECREF(pCTypesModule);
        return nullptr;
    }

    // introduce a new module to execute the Python converter script
    PyObject* pModule = PyImport_AddModule("_ba_converter_"); // borrowed reference
    PyObject* pGlobalDict = PyModule_GetDict(pModule);  // borrowed reference
    PyDict_SetItemString(pGlobalDict, "ctypes", pCTypesModule);
    PyDict_SetItemString(pGlobalDict, "np", pNumpyModule);

    // create the converter function in Python
    // (numpy-array) -> (void-pointer, size, nr-of-dimensions, dimensions, dtype)
    const char pArrayConverterScript[] =
        "def ArrayConverter(np_array):\n"
        "    arr_ctype = np.ctypeslib.as_ctypes_type(np_array.dtype);"
        "    array_ptr_t = ctypes.POINTER(arr_ctype);"
        "    # get the pointer to the raw array data\n"
        "    arr_ptr = np_array.ctypes.data_as(array_ptr_t);"
        "    arr_void_ptr = ctypes.cast(arr_ptr, ctypes.c_void_p);"
        "    # get the size and the dimensions of the array\n"
        "    arr_size = np_array.size;"
        "    arr_dims = tuple(np_array.shape);"
        "    arr_ndims = len(arr_dims);"
        "    arr_dtype = np_array.dtype.name;"
        "    return (arr_void_ptr.value, arr_size, arr_ndims, arr_dims, arr_dtype)";

    // execute the function in the module namespace
    PyObject* pExeResult = PyRun_String(pArrayConverterScript,
                                        Py_file_input, pGlobalDict, pGlobalDict);

    if (pExeResult == NULL) {
        PyErr_Print();
        std::cerr << "Failed to run the Python array-converter script" << std::endl;
        return nullptr;
    }

    Py_DECREF(pExeResult);

    // get the function object
    PyObject* pConverterFunc = PyDict_GetItemString(pGlobalDict, "ArrayConverter");

    // call the converter function with the Numpy array as argument
    PyObject* pResult = PyObject_CallFunctionObjArgs(pConverterFunc, numpyArray, NULL);
    Py_DECREF(pConverterFunc);
    Py_DECREF(pNumpyModule);
    Py_DECREF(pCTypesModule);

    return pResult;
}


PyObject* getPythonArray(const char module_name[], const char function_name[],
                         size_type& arr_size, size_type& arr_ndims, size_type*& arr_dims,
                         std::string& arr_dtype, void*& array_ptr)
{
    // requires ctypes and Numpy libraries

    // load the Python module
    PyObject* pModule = PyImport_ImportModule(module_name);
    if (pModule == nullptr) {
        std::cerr << "C++: failed to load Python module '"
                  << module_name << "'" << std::endl;
        return nullptr;
    }

    // signature of the Python function:
    //   fn() => (void-pointer, array-size, number-of-dimensions, array-dimensions)
    PyObject* pFunc = PyObject_GetAttrString(pModule, function_name);
    if (pFunc == nullptr) {
        std::cerr << "C++: failed to load Python function '"
                  << function_name << "'" << std::endl;
        Py_DECREF(pModule);
        return nullptr;
    }

    // call the Python function and access the result
    PyObject* pArrayDescriptorTuple = PyObject_CallObject(pFunc, nullptr);
    if (pArrayDescriptorTuple == nullptr) {
        std::cerr << "C++: failed to call Python function '"
                  << function_name << "'" << std::endl;
        Py_DECREF(pFunc);
        Py_DECREF(pModule);
        return nullptr;
    }

    parseArrayDescriptor(pArrayDescriptorTuple,
                         arr_size, arr_ndims, arr_dims, arr_dtype, array_ptr);
    // clean up
    Py_DECREF(pFunc);
    Py_DECREF(pModule);

    return pArrayDescriptorTuple;
}


PyObject* getPythonArrayfromFunction(const char module_name[], const char function_name[],
                                     size_type& arr_size, size_type& arr_ndims,
                                     size_type*& arr_dims, std::string& arr_dtype, void*& array_ptr)
{
    // requires ctypes and Numpy libraries

    // load the Python module
    PyObject* pModule = PyImport_ImportModule(module_name);
    if (pModule == nullptr) {
        std::cerr << "C++: failed to load Python module '"
                  << module_name << "'" << std::endl;
        return nullptr;
    }

    // signature of the Python function:
    //   fn() => Numpy-array
    PyObject* pFunc = PyObject_GetAttrString(pModule, function_name);
    if (pFunc == nullptr) {
        std::cerr << "C++: failed to load Python function '"
                  << function_name << "'" << std::endl;
        Py_DECREF(pModule);
        return nullptr;
    }

    PyObject* numpyArray = PyObject_CallObject(pFunc, nullptr);
    Py_DECREF(pModule);
    Py_DECREF(pFunc);

    PyObject* array_descriptor = makeArrayDescriptor(numpyArray);

    if (array_descriptor == nullptr) {
        PyErr_Print();
        std::cerr << "C++: failed to call Python array-converter function"
                  << std::endl;
        return nullptr;
    }

    // pass the Numpy array to the converter
    parseArrayDescriptor(array_descriptor, arr_size, arr_ndims, arr_dims,
                         arr_dtype, array_ptr);
    Py_DECREF(array_descriptor);

    return numpyArray;
}


int main() {
    typedef int32_t array_t;

    Py_Initialize();

    const char module_name[] = "my_module";
    const char function_name[] = "fn";

    std::string dtype;
    size_type size, ndims;
    size_type* dims;
    void* raw_array;
    PyObject* pResult = getPythonArray(module_name, function_name,
                                       size, ndims, dims, dtype, raw_array);

    std::cout << "array metadata: size = " << size
              << ", ndims = " << ndims
              << ", dtype = '" << dtype << "'"
              << std::endl;

    for (size_type i = 0; i < ndims; ++i)
        std::cout << "dim[" << i << "] = " << dims[i] << ", ";
    std::cout << std::endl;

    auto array = reinterpret_cast<array_t*>(raw_array);
    if (array != nullptr) {
        std::cout << "C++: received array from Python (1):" << std::endl;
        for (size_type i = 0; i < size; ++i)
            std::cout << array[i] << " ";

        std::cout << std::endl;
    };

    // clean up
    if (ndims > 0)
        delete[] dims;

    Py_XDECREF(pResult);
    std::cout << "------------------------------" << std::endl;

    raw_array = nullptr;
    pResult = getPythonArrayfromFunction(module_name, "fn_array",
                                         size, ndims, dims, dtype, raw_array);

    std::cout << "array metadata: size = " << size
              << ", ndims = " << ndims
              << ", dtype = '" << dtype << "'"
              << std::endl;

    for (size_type i = 0; i < ndims; ++i)
        std::cout << "dim[" << i << "] = " << dims[i] << ", ";
    std::cout << std::endl;

    array = reinterpret_cast<array_t*>(raw_array);
    if (array != nullptr) {
        std::cout << "C++: received array from Python (2):" << std::endl;
        for (size_type i = 0; i < size; ++i)
            std::cout << array[i] << " ";

        std::cout << std::endl;
    };

    // clean up
    if (ndims > 0)
        delete[] dims;

    Py_XDECREF(pResult);
    std::cout << "------------------------------" << std::endl;

    Py_Finalize();
    return 0;
}
