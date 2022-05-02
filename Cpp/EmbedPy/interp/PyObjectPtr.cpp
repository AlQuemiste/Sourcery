#include "PyObjectPtr.h"
#include "PythonInterpreter.h"

#include <string>
#include <sstream>  // stringstream
#include <exception>

#include <iostream> // DEBUG


Status::Status(const Status::Type type, const std::string& message):
    type{type},
    message{message} {}

bool Status::isSet() const
{
    return (type != Status::Type::None);
}


bool Status::isOK() const
{
    return (type == Status::Type::Info);
}


bool Status::isWarning() const
{
    return (type == Status::Type::Warning);
}


bool Status::isError() const
{
    return (type == Status::Type::Error);
}


Status::operator bool() const
{
    return isOK();
}

//========================================

PyObjectPtr::PyObjectPtr(PyObject* pyobject_ptr,
                         const Status& status):
    ptr{pyobject_ptr},
    status{status}
{
    // if PyObject pointer is null but result type is not `None` or `Error`,
    // then that will be an `Error` case.
    if(!ptr && status.type != Status::Type::None
       && status.type != Status::Type::Error)
    {
        this->status.type = Status::Type::Error;
    }

    std::cout << "Init PyObjectPtr<" << this << ">" << std::endl;
}


PyObjectPtr::PyObjectPtr(PyObject* pyobject_ptr):
    PyObjectPtr(pyobject_ptr, Status(Status::Type::Info)) {}


PyObjectPtr::PyObjectPtr(const Status& status):
    PyObjectPtr(nullptr, status) {}


PyObjectPtr::~PyObjectPtr()
{
    discard();
}


PyObjectPtr::PyObjectPtr(PyObjectPtr&& other):
    PyObjectPtr(other.ptr, other.status)
{
    // reset the moved object
    other.reset();
}


void PyObjectPtr::reset()
{
    ptr = nullptr;
    status = Status();
}


void PyObjectPtr::discard()
{
    if (!PythonInterpreter::isInitialized()) {
        throw(std::runtime_error(
                  "Decrementing Python reference-count without "
                  "Python initialized leads to memory access violation "
                  "(segmentation fault)"));
    }

    PythonInterpreter::DecRef(ptr);
    reset();
    std::cout << "DONE Discard PyObjectPtr<" << this << ">" << std::endl;
}


bool PyObjectPtr::valid() const
{
    return (ptr && (status.type == Status::Type::Info
                    || status.type == Status::Type::Warning));
}
