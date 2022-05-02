#ifndef PYOBJECTPTR_H
#define PYOBJECTPTR_H

#include "PyObjectDecl.h"
#include <string>

// ========================================

// Indicator for the status of a result
struct Status
{
    enum class Type {None=-1, Info, Warning, Error};
    Type type = Type::None;

    // status message
    std::string message;

    Status(const Status::Type type = Type::Info,
           const std::string& message = "");

    bool isSet() const;

    bool isOK() const;

    bool isWarning() const;

    bool isError() const;

    // cast to boolean (true if result is valid)
    operator bool() const;
};

//========================================

// safe container for PyObjects
class PyObjectPtr
{

public:
    // raw pointer to the PyObject
    PyObject* ptr = nullptr;

    // status of the PyObject
    Status status;

    PyObjectPtr(PyObject* pyobject_ptr, const Status& status);

    PyObjectPtr(PyObject* pyobject_ptr);

    PyObjectPtr(const Status& status);

    ~PyObjectPtr();

    // disallow copy constructor
    PyObjectPtr(const PyObjectPtr&) = delete;
    // disallow copy assignment
    PyObjectPtr& operator=(const PyObjectPtr&) = delete;
    // allow move constructor
    PyObjectPtr(PyObjectPtr&& other);
    // reset the container to the initial status (does _not_ release the Python resource)
    void reset();
    // discards the Python resource (decrements the Python reference)
    void discard();
    // check validity of the PyObjectPtr
    bool valid() const;
};

#endif  // PYOBJECTPTR_H
