#include <Python.h>
#include <string>
#include <map>
#include <variant>

#include <iostream>

using ValueType = std::variant<long int, double, std::string>;
using DictMap = std::map<std::string, ValueType>;

DictMap parsePythonDict(PyObject* pyDict) {
    DictMap result;

    if (!PyDict_Check(pyDict)) {
        throw std::runtime_error("C++: input is not a Python dictionary");
    }

    PyObject* key;
    PyObject* value;
    Py_ssize_t pos = 0;

    while (PyDict_Next(pyDict, &pos, &key, &value)) {
        const std::string keyStr = PyUnicode_AsUTF8(key);

        if (PyLong_Check(value)) {
            result[keyStr] = PyLong_AsLong(value);
        } else if (PyFloat_Check(value)) {
            result[keyStr] = PyFloat_AsDouble(value);
        } else if (PyUnicode_Check(value)) {
            result[keyStr] = PyUnicode_AsUTF8(value);
        } else if (value == Py_None) {
            result[keyStr] = std::string("None");
        } else {
            std::cerr << "C++: unsupported type for key: " << keyStr << std::endl;
        }
    }

    return result;
}


int main() {
    Py_Initialize();

    // produce a Python dictionary
    PyObject* pDict = PyDict_New();
    PyDict_SetItemString(pDict, "NumHigh", PyLong_FromLong(0));
    PyDict_SetItemString(pDict, "Format", PyUnicode_FromString("compressed"));
    PyDict_SetItemString(pDict, "Mode", PyUnicode_FromString("Time"));
    PyDict_SetItemString(pDict, "NumPixels", PyUnicode_FromString("5290000"));
    PyDict_SetItemString(pDict, "PixelLength", PyFloat_FromDouble(0.15));
    PyDict_SetItemString(pDict, "desc", Py_None);

    try {
        DictMap dictMap = parsePythonDict(pDict);

        // Print the contents of the C++ map
        for (const auto& [key, value] : dictMap) {
            std::cout << key << ": ";
            std::visit([](const auto& v) { std::cout << v; }, value);
            std::cout << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    Py_DECREF(pDict);

    Py_Finalize();
    return 0;
}
