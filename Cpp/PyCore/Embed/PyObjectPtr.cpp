//  ************************************************************************************************
//
//  BornAgain: simulate and fit reflection and scattering
//
//! @file      PyCore/Embed/PyObjectPtr.cpp
//! @brief     Implements class PyObjectPtr.
//!
//! @homepage  http://www.bornagainproject.org
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @copyright Forschungszentrum Jülich GmbH 2018
//! @authors   Scientific Computing Group at MLZ (see CITATION, AUTHORS)
//
//  ************************************************************************************************

#include "PyObjectPtr.h"
#include "PyInterpreter.h"

PyObjectPtr::PyObjectPtr(PyObjectPtr&& other)
    : PyObjectPtr(other.release())
{
}

void PyObjectPtr::reset()
{
    m_ptr = nullptr;
}

PyObject* PyObjectPtr::release()
{
    PyObject* pyobject_ptr{m_ptr};
    reset();
    return pyobject_ptr;
}

void PyObjectPtr::discard()
{
    if (m_borrowed) {
        // borrowed reference need no reference decrement
        reset();
        return;
    }

    PyInterpreter::DecRef(m_ptr);
    reset();
}


NumpyArrayWrapper::NumpyArrayWrapper(const std::size_t datasize, const std::size_t n_dims,
                                     const std::size_t* dims, const std::string& dtype,
                                     const void* array_ptr, PyObject* py_array_ptr,
                                     const ArrayMetadata& metadata)
    : m_datasize{datasize}
    , m_dtype{dtype}
    , m_metadata{metadata}
    , m_array_ptr{array_ptr}
    , m_py_array_ptr{py_array_ptr}
{
    // check validity
    if (m_datasize < 1 || n_dims < 1 || !dims || !m_array_ptr || !m_py_array_ptr)
        return;

    for (std::size_t i_d = 0; i_d < n_dims; ++i_d)
        if (dims[i_d] < 1)
            return;

    // initialize the array descriptor
    m_dims.resize(n_dims);
    for (std::size_t i_d = 0; i_d < n_dims; ++i_d)
        m_dims[i_d] = dims[i_d];
}

NumpyArrayWrapper::~NumpyArrayWrapper()
{
    if (m_py_array_ptr)
        PyInterpreter::DecRef(m_py_array_ptr);
}
