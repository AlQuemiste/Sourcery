//  ************************************************************************************************
//
//  BornAgain: simulate and fit reflection and scattering
//
//! @file      PyCore/Embed/PyInterpreter.h
//! @brief     Declares functions to expose Python-interpreter functionality to C++.
//!
//! @homepage  http://www.bornagainproject.org
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @copyright Forschungszentrum Jülich GmbH 2018
//! @authors   Scientific Computing Group at MLZ (see CITATION, AUTHORS)
//
//  ************************************************************************************************

#ifndef BORNAGAIN_PYTHON
#error this header requires Python support
#endif
#ifndef BORNAGAIN_PYCORE_EMBED_PYINTERPRETER_H
#define BORNAGAIN_PYCORE_EMBED_PYINTERPRETER_H

#include "PyCore/Embed/PyObjectDecl.h"
#include "PyCore/Embed/PyObjectPtr.h"
#include <string>
#include <vector>

class PyObjectPtr;

namespace PyInterpreter {

// Python stable ABI
void initialize();

// Python stable ABI
bool isInitialized();

// Python stable ABI
void finalize();

// Python stable ABI
bool checkError();

// Python stable ABI
void addPythonPath(const std::string& path);

// Python stable ABI
void setPythonPath(const std::string& path);

// Python stable ABI
PyObjectPtr import(const std::string& pymodule_name, const std::string& path = "");
// Python stable ABI
void DecRef(PyObject* py_object);

//! Converts String PyObject into string, if possible, or throws exception
std::string pyStrtoString(PyObject* obj);

//! Gets an attribute from a Python module
std::string strAttribute(PyObject* py_module, const std::string& attr_name);

//! Calls a Python module function with a given name and a signature `fn() -> str`
void callFunction(PyObject* py_module, const std::string& fn_name);

//! Returns multi-line string representing PATH, PYTHONPATH, sys.path and other info.
std::string runtimeInfo();

//! Returns string representing python stack trace.
std::string stackTrace();

//! Returns the full error description via Python stack trace
std::string errorDescription(const std::string& title = "");

//! Returns the full error description via Python stack trace and throws and exception
void terminateOnError(const std::string& message = "");

// BornAgain-related functionality
namespace BornAgain {

//! Imports BornAgain from given location. If path is empty, tries to rely on PYTHONPATH.
PyObjectPtr import(const std::string& path = "");

//! Imports a 'BornAgain' Python script
PyObjectPtr importScript(const std::string& script, const std::string& path);

//! Returns list of functions defined in the script, given a script filename and
//! a path to import BornAgain library (if empty, relies on `PYTHONPATH`).
std::vector<std::string> listOfFunctions(const std::string& script, const std::string& path);

//! Call a BornAgain-related function and return its returned value
PyObjectPtr callScriptFunction(const std::string& functionName, const std::string& script,
                               const std::string& path);

} // namespace BornAgain

// Fabio-related functionality
// see: Knudsen et al, J. Appl. Cryst. 46, (2013) 537-539
// <https://doi.org/10.1107/S0021889813000150>

namespace Fabio {

//! Imports 'fabio' Python module
PyObjectPtr import();
//! Loads the data from a given filename via Fabio (needs Numpy)
NumpyArrayWrapper load(const std::string& filename);

} // namespace Fabio

} // namespace PyInterpreter

#endif // BORNAGAIN_PYCORE_EMBED_PYINTERPRETER_H
