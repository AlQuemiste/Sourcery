//  ************************************************************************************************
//
//  BornAgain: simulate and fit reflection and scattering
//
//! @file      PyCore/Embed/PyInterpreter.cpp
//! @brief     Implements functions to expose Python-interpreter functionality to C++.
//!
//! @homepage  http://www.bornagainproject.org
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @copyright Forschungszentrum Jülich GmbH 2018
//! @authors   Scientific Computing Group at MLZ (see CITATION, AUTHORS)
//
//  ************************************************************************************************

/* Embedded Python Interpreter

Note that Python objects are structures allocated on the heap,
accessed through pointers of type `PyObject*`.

References:
- Python C-API <https://docs.python.org/3/c-api>
- Python ABI stability <https://docs.python.org/3/c-api/stable.html>
- Numpy C-API <https://numpy.org/doc/stable/reference/c-api>;
  <https://numpy.org/doc/stable/reference/c-api/array.html>.
- Python Extension Patterns <https://pythonextensionpatterns.readthedocs.io>
- "Python behind the scenes" series <https://tenthousandmeters.com/tag/python-behind-the-scenes>
- Python's garbage collector <https://rushter.com/blog/python-garbage-collector>
*/

#include "PyCore/Embed/PyInterpreter.h"
#include "Base/Util/Assert.h"
#include "PyCore/Embed/PyCore.h"
#include <algorithm> // transform
#include <chrono>    // time_since_epoch
#include <csignal>   // SIGINT
#include <cstddef>   // NULL
#include <cstring>   // memcpy
#include <iostream>  // cerr
#include <memory>    // unique_ptr
#include <stdexcept> // runtime_error

// unique id used to name the temporary Python modules
static int uid = -1;

// separator line for error messages
static const std::string sep_line = std::string(30, '-');

namespace {

std::string wToString(const wchar_t* const s)
{
    if (!s)
        return "";
    std::wstring w(s);
    std::string result(w.length(), 0);
    std::transform(w.begin(), w.end(), result.begin(), [](wchar_t c) { return (char)c; });
    return result;
}

//! Converts PyObject into vector of strings, if possible, or throws exception
std::vector<std::string> toVectorString(PyObject* py_object)
{
    std::vector<std::string> result;

    if (PyTuple_Check(py_object)) {
        for (Py_ssize_t i = 0; i < PyTuple_Size(py_object); i++) {
            PyObject* value = PyTuple_GetItem(py_object, i); // borrowed reference
            result.push_back(PyInterpreter::pyStrtoString(value));
        }
    } else if (PyList_Check(py_object)) {
        for (Py_ssize_t i = 0; i < PyList_Size(py_object); i++) {
            PyObject* value = PyList_GetItem(py_object, i); // borrowed reference
            result.push_back(PyInterpreter::pyStrtoString(value));
        }
    } else {
        throw std::runtime_error(PyInterpreter::errorDescription(
            "PyInterpreter: Cannnot convert the given Python object "
            "to vector<string>."));
    }

    return result;
}

std::string getId()
{
    uid += 1;
    return std::to_string(uid);
}

} // namespace


// NOTE: "Python stable ABI" denotes the functions which use _only_ the Python's stable ABI;
// see <https://docs.python.org/3/c-api/stable.html>

// Python stable ABI
void PyInterpreter::initialize()
{
    if (!Py_IsInitialized())
        Py_Initialize();
}

// Python stable ABI
bool PyInterpreter::isInitialized()
{
    return static_cast<bool>(Py_IsInitialized());
}

// Python stable ABI
void PyInterpreter::finalize()
{
    // undo all initializations made by Py_Initialize() and subsequent use
    // of Python/C API functions, and destroy all sub-interpreters.
    // This is a no-op when called for a second time.
    Py_Finalize();
}

// Python stable ABI
bool PyInterpreter::checkError()
{
    if (PyErr_Occurred()) {
        PyErr_Clear();
        return true;
    }
    return false;
}

// Python stable ABI
void PyInterpreter::addPythonPath(const std::string& path)
{
    if (!path.empty()) {
        PyObject* sysPath = PySys_GetObject("path");                // borrowed reference
        PyList_Append(sysPath, PyUnicode_FromString(path.c_str())); // add to `PYTHONPATH`
    }
}

// Python stable ABI
void PyInterpreter::setPythonPath(const std::string& path)
{
    // returns 0 on success, -1 on error
    const int result = PySys_SetObject((char*)"path", PyUnicode_FromString(path.c_str()));
    if (result != 0)
        PyInterpreter::terminateOnError("PyInterpreter: Cannot set the Python path");
}

// Python stable ABI
PyObjectPtr PyInterpreter::import(const std::string& pymodule_name, const std::string& path)
{
    ASSERT(!pymodule_name.empty());

    addPythonPath(path);

    // import the module
    PyObject* pymodule = PyImport_ImportModule(pymodule_name.c_str());
    if (!pymodule || !PyModule_Check(pymodule))
        PyInterpreter::terminateOnError("PyInterpreter: Cannot load Python module '" + pymodule_name
                                        + "' (given path = '" + path + "')");

    // returns a _new_ reference; ie. caller is responsible for the ref-count
    return pymodule;
}

// Python stable ABI
void PyInterpreter::DecRef(PyObject* py_object)
{
    // Decrementing Python reference-count without
    // Python initialized leads to memory access violation (segmentation fault)
    ASSERT(PyInterpreter::isInitialized());

    Py_XDECREF(py_object);
}

std::string PyInterpreter::pyStrtoString(PyObject* py_object)
{
    std::string result;
    PyObject* pyStr = PyUnicode_AsEncodedString(py_object, "utf-8", "replace");
    if (pyStr) {
        result = std::string(PyBytes_AsString(pyStr));
        Py_DecRef(pyStr);
    }

    return result;
}

std::string PyInterpreter::strAttribute(PyObject* py_module, const std::string& attr_name)
{
    std::string result_str;
    PyObject* pAttr = PyObject_GetAttrString(py_module, attr_name.c_str());

    if (!pAttr) {
        Py_DecRef(pAttr);
        PyInterpreter::terminateOnError("PyInterpreter: Cannot get Python attribute '" + attr_name
                                        + "'");
    }

    result_str = PyInterpreter::pyStrtoString(pAttr);
    Py_DecRef(pAttr);
    return result_str;
}

void PyInterpreter::callFunction(PyObject* py_module, const std::string& fn_name)
{

    // NOTE: The Python function signature is assumed to be `fn() -> str`
    std::string result_str;
    PyObject *pFunc, *pRetValue;

    pFunc = PyObject_GetAttrString(py_module, fn_name.c_str());

    if (!pFunc || !PyCallable_Check(pFunc)) {
        Py_DecRef(pFunc);
        PyInterpreter::terminateOnError("PyInterpreter: Cannot call Python function '" + fn_name
                                        + "'");
    }

    // Call the function with no arguments
    pRetValue = PyObject_CallObject(pFunc, NULL);
    result_str = PyInterpreter::pyStrtoString(pRetValue);
    Py_DecRef(pRetValue);
    Py_DecRef(pFunc);
}

std::string PyInterpreter::runtimeInfo()
{
    std::string result;

    // Embedded Python details
    {
        PyConfig config;
        PyConfig_InitPythonConfig(&config);
        auto status = PyConfig_Read(&config);
        if (PyStatus_Exception(status))
            throw std::runtime_error("Cannot access PyConfig");
        result += "  - program name: " + wToString(config.program_name) + "\n";
        PyConfig_Clear(&config);
    }
    {
        PyObject* obj = PySys_GetObject("executable");
        PyObject* repr = PyObject_Str(obj);
        const std::string s = PyUnicode_AsUTF8AndSize(repr, NULL);
        result += "  - executable: " + s + "\n";
    }
    {
        PyObject* obj = PySys_GetObject("path");
        PyObject* repr = PyObject_Str(obj);
        const std::string s = PyUnicode_AsUTF8AndSize(repr, NULL);
        result += "  - Python PATH: " + s + "\n";
    }
    {
        // TODO WAIT
        // This can be replaced by PyConfig_Get("home"),
        // once the Python C API is at version >= 3.14
        // See https://github.com/python/cpython/issues/129342#issuecomment-2615759261.
        PyConfig config;
        PyConfig_InitPythonConfig(&config);
        auto status = PyConfig_Read(&config);
        if (PyStatus_Exception(status))
            throw std::runtime_error("Cannot access PyConfig");
        result += "  - Python home: " + wToString(config.home) + "\n";
        PyConfig_Clear(&config);
    }

    // Runtime Python's sys.path
    PyObject* sysPath = PySys_GetObject((char*)"path"); // borrowed reference
    std::vector<std::string> content{toVectorString(sysPath)};
    result += "  - sys.path: ";
    for (const std::string& s : content)
        result += s + ",";

    return result;
}

// Attempt to retrieve Python stack trace
// Ref: <https://stackoverflow.com/a/15907460>
std::string PyInterpreter::stackTrace()
{
    std::string result;

    if (!PyErr_Occurred())
        return "";

    PyObject *ptype, *pvalue, *ptraceback, *pstr;

    // fetch error indicators
    PyErr_Fetch(&ptype, &pvalue, &ptraceback);

    if (!ptype) {
        Py_XDECREF(pvalue);
        Py_XDECREF(ptraceback);
        return "";
    }

    if (pvalue) {
        pstr = PyObject_Str(pvalue);
        if (const char* str = PyUnicode_AsUTF8(pstr))
            result += std::string(str) + "\n";

        Py_XDECREF(pstr);
    }

    if (ptraceback) {
        // import traceback module and add Python traceback
        PyObject* py_traceback_module = PyImport_ImportModule("traceback");

        if (py_traceback_module) {
            // format the stack trace and the exception information
            PyObject* py_format_exc =
                PyObject_GetAttrString(py_traceback_module, "format_exception");
            if (py_format_exc && PyCallable_Check(py_format_exc)) {
                PyObject* ptraceback_list =
                    PyObject_CallFunctionObjArgs(py_format_exc, ptype, pvalue, ptraceback, NULL);
                if (ptraceback_list) {
                    // join the traceback lines
                    for (Py_ssize_t i = 0; i < PyList_Size(ptraceback_list); ++i) {
                        PyObject* line = PyList_GetItem(ptraceback_list, i); // borrowed reference
                        result += "\n" + std::string(PyUnicode_AsUTF8(line));
                    }
                    Py_DecRef(ptraceback_list);
                }
            }

            Py_XDECREF(py_format_exc);
        }

        Py_XDECREF(py_traceback_module);
    }

    result += "\n";
    result += PyInterpreter::runtimeInfo();

    return result;
}

std::string PyInterpreter::errorDescription(const std::string& title)
{
    std::string msg =
        title + "\n" + sep_line + "\n" + "* Python exception:\n" + PyInterpreter::stackTrace();
    return msg;
}

void PyInterpreter::terminateOnError(const std::string& message)
{
    throw std::runtime_error(errorDescription(message));
}


PyObjectPtr PyInterpreter::BornAgain::import(const std::string& path)
{
    PyInterpreter::addPythonPath(path);

#ifndef _WIN32
    // store ctrl-C handler before Numpy messes it up
    PyOS_sighandler_t sighandler = PyOS_getsig(SIGINT);
#endif

    PyObject* ba_pymodule = PyImport_ImportModule("bornagain");

#ifndef _WIN32
    PyOS_setsig(SIGINT, sighandler); // restore previous ctrl-C handler
#endif

    if (!ba_pymodule || !PyModule_Check(ba_pymodule))
        PyInterpreter::terminateOnError(
            "PyInterpreter:BornAgain: Cannot load 'bornagain' Python module "
            "(given path = '"
            + path + "')");

    return ba_pymodule;
}

PyObjectPtr PyInterpreter::BornAgain::importScript(const std::string& script,
                                                   const std::string& path)
{
    // import bornagain Python module
    PyObjectPtr pBAModule{PyInterpreter::BornAgain::import(path)};

    // introduce a new module to execute the Python script
    const std::string tmp_module_name = "_ba_tmp_module_" + getId() + "_";
    PyObject* pModule = PyImport_AddModule(tmp_module_name.c_str()); // borrowed reference
    if (pModule == NULL)
        PyInterpreter::terminateOnError(
            "PyInterpreter:BornAgain: Cannot add a temporary Python module");

    PyObject* pGlobalDict = PyModule_GetDict(pModule); // borrowed reference
    PyDict_SetItemString(pGlobalDict, "bornagain", pBAModule.get());

    // execute the function in the module namespace
    PyObject* pExeResult = PyRun_String(script.c_str(), Py_file_input, pGlobalDict, pGlobalDict);

    if (pExeResult == NULL)
        PyInterpreter::terminateOnError("PyInterpreter:BornAgain: Failed to run the script");

    Py_DecRef(pExeResult);

    return PyObjectPtr{pModule, /* borrowed = */ true};
}


PyObjectPtr PyInterpreter::BornAgain::callScriptFunction(const std::string& functionName,
                                                         const std::string& script,
                                                         const std::string& path)
{
    PyObjectPtr tmpModule{PyInterpreter::BornAgain::importScript(script, path)};

    // locate the function (it is an attribute of the module)
    PyObject* pFunc = PyObject_GetAttrString(tmpModule.get(), functionName.c_str());
    if (!pFunc)
        PyInterpreter::terminateOnError("PyInterpreter:BornAgain: "
                                        "Cannot locate the compiled function\n'''\n"
                                        + functionName + "\n'''");

    // call the function with signature f() -> PythonObject
    PyObject* pResult = PyObject_CallFunctionObjArgs(pFunc, NULL);
    Py_DecRef(pFunc);

    if (!pResult)
        PyInterpreter::terminateOnError("PyInterpreter:BornAgain: "
                                        "Failed executing Python function '"
                                        + functionName + "'");

    return pResult;
}

std::vector<std::string> PyInterpreter::BornAgain::listOfFunctions(const std::string& script,
                                                                   const std::string& path)
{
    PyObjectPtr tmpModule{PyInterpreter::BornAgain::importScript(script, path)};

    if (!tmpModule.valid())
        PyInterpreter::terminateOnError("PyInterpreter:BornAgain: "
                                        "Cannot obtain the dictionary from the script module");

    PyObject* pDict = PyModule_GetDict(tmpModule.get()); // borrowed reference
    if (!pDict)
        PyInterpreter::terminateOnError("PyInterpreter:BornAgain: "
                                        "Cannot obtain the dictionary from the script module");

    PyObject *key, *value;
    Py_ssize_t pos = 0;
    std::vector<std::string> fn_names;
    while (PyDict_Next(pDict, &pos, &key, &value)) {
        if (PyCallable_Check(value)) {
            std::string func_name{PyInterpreter::pyStrtoString(key)};
            // do not import callables whose names begin with '__'
            if (func_name.find("__") == std::string::npos)
                fn_names.push_back(func_name);
        }
    }

    return fn_names;
}

//----------------------------------------

//! Fabio-related auxiliary functions
namespace {

//! Parses a given Python dictionary to construct C++ map from the items
ArrayMetadata parsePythonDict(PyObject* const pyDict)
{
    ArrayMetadata result_map;

    if (!PyDict_Check(pyDict))
        PyInterpreter::terminateOnError("PyInterpreter:Fabio: Input is not a Python dictionary");

    PyObject* key;
    PyObject* value;
    Py_ssize_t pos = 0;

    while (PyDict_Next(pyDict, &pos, &key, &value)) {
        const std::string keyStr = PyUnicode_AsUTF8(key);

        if (PyLong_Check(value))
            result_map[keyStr] = PyLong_AsLong(value);
        else if (PyFloat_Check(value))
            result_map[keyStr] = PyFloat_AsDouble(value);
        else if (PyUnicode_Check(value))
            result_map[keyStr] = PyUnicode_AsUTF8(value);
        else if (value == Py_None)
            result_map[keyStr] = std::string("<None>");
        else {
            // attempt to extract a string-representation of the object
            PyObject* value_str = PyObject_Str(value);
            if (PyUnicode_Check(value_str))
                result_map[keyStr] = PyUnicode_AsUTF8(value_str);

            Py_XDECREF(value_str);
        }
    }

    return result_map;
}

//! Makes a descriptor for a given Numpy array (needs ctypes and numpy Python packages)
PyObject* makeArrayDescriptor(PyObject* const numpyArray)
{
    // introduce a new module to execute the Python converter script
    const std::string _module_name = "_ba_converter_" + getId() + "_";
    PyObject* pModule = PyImport_AddModule(_module_name.c_str()); // borrowed reference
    PyObject* pGlobalDict = PyModule_GetDict(pModule);            // borrowed reference

    // create the converter function in Python
    // (numpy-array) -> (void-pointer, size, nr-of-dimensions, dimensions, dtype)
    const char pArrayConverterScript[] =
        "def ArrayConverter(np_array):\n"
        "    import numpy as np, ctypes;"
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
    PyObject* pExeResult =
        PyRun_String(pArrayConverterScript, Py_file_input, pGlobalDict, pGlobalDict);

    if (!pExeResult)
        PyInterpreter::terminateOnError("PyInterpreter:Fabio: "
                                        "Failed to run the Python array-converter script");

    Py_DECREF(pExeResult);

    // get the function object
    PyObject* pConverterFunc =
        PyDict_GetItemString(pGlobalDict, "ArrayConverter"); // borrowed reference

    // call the converter function with the Numpy array as argument
    PyObject* pResult = PyObject_CallFunctionObjArgs(pConverterFunc, numpyArray, NULL);

    if (!pResult)
        PyInterpreter::terminateOnError(
            "PyInterpreter:Fabio: Python array-converter script did not yield a proper result");

    // Python result: tuple(void-ptr, size, ndims, dims, dtype)
    return pResult;
}

//! Retrieves the array description from a given Python data-structure
void parseArrayDescriptor(PyObject* const pyArrayDescriptorTuple, std::size_t& arr_size,
                          std::size_t& arr_ndims, std::size_t*& arr_dims, std::string& arr_dtype,
                          void*& array_ptr)
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

    if (!PyArg_ParseTuple(pyArrayDescriptorTuple, "OOOOO", // borrowed reference
                          &py_void_ptr, &py_arr_size, &py_ndims, &py_arr_dims, &py_arr_dtype))
        PyInterpreter::terminateOnError(
            "PyInterpreter:Fabio: "
            "Failed to parse returned values from Python array-converter");

    if (!(py_void_ptr && py_arr_size && py_ndims && py_arr_dims && py_arr_dtype))
        PyInterpreter::terminateOnError("PyInterpreter:Fabio: "
                                        "Invalid returned values from Python array-converter");

    // array void-pointer and metadata
    array_ptr = PyLong_AsVoidPtr(py_void_ptr);
    arr_size = static_cast<std::size_t>(PyLong_AsLong(py_arr_size));
    arr_ndims = static_cast<std::size_t>(PyLong_AsLong(py_ndims));
    // convert 'dtype' (Python string) to std::string
    PyObject* _dtype = PyUnicode_AsEncodedString(py_arr_dtype, "UTF-8", "strict");
    arr_dtype = std::string(PyBytes_AsString(_dtype));
    Py_DECREF(_dtype);

    // verify array properties
    if (!(array_ptr && arr_size > 0 && arr_ndims > 0 && !arr_dtype.empty()))
        return;

    // store each dimension of the array
    arr_dims = new std::size_t[arr_ndims];
    for (std::size_t i_d = 0; i_d < arr_ndims; ++i_d)
        arr_dims[i_d] = static_cast<std::size_t>(PyLong_AsLong(PyTuple_GetItem(py_arr_dims, i_d)));
}


//! Use Python fabio to read data and to obtain a Numpy array and the Fabio header
ArrayMetadata _fabio_load(const std::string& filename, PyObject*& numpyArray)
{
    // import fabio
    PyObject* pFabioModule = PyImport_ImportModule("fabio");
    if (pFabioModule == NULL)
        PyInterpreter::terminateOnError("PyInterpreter:Fabio: Failed to import fabio");

    // introduce a new module to execute the Python converter script
    const std::string _module_name = "_ba_fabio_" + getId() + "_";
    PyObject* pModule = PyImport_AddModule(_module_name.c_str()); // borrowed reference
    PyObject* pGlobalDict = PyModule_GetDict(pModule);            // borrowed reference
    // add fabio to the global-dict of the module
    PyDict_SetItemString(pGlobalDict, "fabio", pFabioModule);

    // create the converter function in Python
    // (data-filename:str) -> numpy-array
    const char pFabioDataImportScript[] = "def import_data(data_filename):\n"
                                          "    im_data = fabio.open(data_filename);"
                                          "    return (im_data.data, im_data.header);";

    // execute the function in the module namespace
    PyObject* pExeResult =
        PyRun_String(pFabioDataImportScript, Py_file_input, pGlobalDict, pGlobalDict);

    if (pExeResult == NULL)
        PyInterpreter::terminateOnError("PyInterpreter:Fabio: "
                                        "Failed to run the Python import_data script");

    Py_DECREF(pExeResult);

    // get the function object
    PyObject* pDataLoadFunc =
        PyDict_GetItemString(pGlobalDict, "import_data"); // borrowed reference
    // convert the filename to Python str
    PyObject* pFilename = PyUnicode_FromString(filename.c_str());
    // call the converter function with the filename as argument;
    // the result must be a Numpy array and a data header
    PyObject* pResult = PyObject_CallFunctionObjArgs(pDataLoadFunc, pFilename, NULL);
    Py_DECREF(pFilename);
    Py_DECREF(pFabioModule);

    PyObject* pFabioHeader;
    if (!pResult || !PyTuple_Check(pResult)) {
        Py_XDECREF(pResult);
        PyInterpreter::terminateOnError("PyInterpreter:Fabio: "
                                        "Python import_data script did not return a proper result");
    }

    if (!PyArg_ParseTuple(pResult, "OO", // borrowed reference
                          &numpyArray, &pFabioHeader)) {
        Py_XDECREF(pResult);
        PyInterpreter::terminateOnError("PyInterpreter:Fabio: "
                                        "Failed to parse returned values from Python import_data");
    }

    if (!(numpyArray && pFabioHeader)) {
        Py_DECREF(pResult);
        Py_XDECREF(numpyArray);
        Py_XDECREF(pFabioHeader);
        PyInterpreter::terminateOnError("PyInterpreter:Fabio: "
                                        "Invalid returned values from Python import_data");
    }

    // increase the reference count to keep the Numpy object alive in the Python interpreter
    Py_IncRef(numpyArray);

    ArrayMetadata fabioHeader{parsePythonDict(pFabioHeader)};
    Py_DECREF(pFabioHeader);

    return fabioHeader;
}

} // namespace


PyObjectPtr PyInterpreter::Fabio::import()
{
    return PyInterpreter::import("fabio");
}

NumpyArrayWrapper PyInterpreter::Fabio::load(const std::string& filename)
{
    PyObject* numpyArray = nullptr;
    ArrayMetadata header = _fabio_load(filename, numpyArray);

    // obtain a descriptor (Python object) for the array
    PyObject* pArrDesc = makeArrayDescriptor(numpyArray);

    // parse the Python array descriptor
    std::size_t arr_size, arr_ndims;
    std::size_t* arr_dims;
    std::string arr_dtype;
    void* array_ptr;

    parseArrayDescriptor(pArrDesc, arr_size, arr_ndims, arr_dims, arr_dtype, array_ptr);

    Py_DECREF(pArrDesc);

    return NumpyArrayWrapper(arr_size, arr_ndims, arr_dims, arr_dtype, array_ptr, numpyArray,
                             header);
}
