//  ************************************************************************************************
//
//  BornAgain: simulate and fit reflection and scattering
//
//! @file      PyCore/Embed/PyObjectPtr.h
//! @brief     Defines class PyObjectPtr.
//!
//! @homepage  http://www.bornagainproject.org
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @copyright Forschungszentrum Jülich GmbH 2018
//! @authors   Scientific Computing Group at MLZ (see CITATION, AUTHORS)
//
//  ************************************************************************************************

#ifdef SWIG
#error no need to expose this header to Swig
#endif // SWIG
#ifndef BORNAGAIN_PYTHON
#error this header requires Python support
#endif
#ifndef BORNAGAIN_PYCORE_EMBED_PYOBJECTPTR_H
#define BORNAGAIN_PYCORE_EMBED_PYOBJECTPTR_H

#include "PyCore/Embed/PyObjectDecl.h"
#include <map>
#include <string>
#include <variant>
#include <vector>

// metadata container

using ValueType = std::variant<long int, double, std::string>;
using ArrayMetadata = std::map<std::string, ValueType>;

//! Safe container for PyObjects:
//! The class `PyObjectPtr` contains a `PyObject*` (or `NULL`).
//! Decrementing Python reference-count is performed automatically when
//! the life-time of a `PyObjectPtr` expires.
class PyObjectPtr {
public:
    PyObjectPtr(PyObject* pyobject_ptr, const bool borrowed = false)
        : m_ptr{pyobject_ptr}
        , m_borrowed{borrowed}
    {
    }

    ~PyObjectPtr() { discard(); }

    PyObjectPtr(const PyObjectPtr&) = delete;
    PyObjectPtr(PyObjectPtr&& other);
    PyObjectPtr& operator=(const PyObjectPtr&) = delete;

    //! Returns the raw pointer to the PyObject
    PyObject* get() { return m_ptr; }
    //! Resets the container to the initial status (does _not_ release the Python resource)
    void reset();
    //! Resets the container to the initial status and return the PyObject pointer
    //! (does _not_ release the Python resource)
    PyObject* release();
    //! Discards the Python resource (decrements the Python reference)
    void discard();
    //! Check the validity of the PyObjectPtr
    bool valid() const { return static_cast<bool>(m_ptr); }

private:
    //! Raw pointer to the PyObject
    PyObject* m_ptr{nullptr};

    //! Flag to determine if PyObject is a borrowed reference
    // NOTE:
    // The Python C-API uses two main types of object references:
    // 1) 'Strong' reference (aka. 'new' reference) is a pointer that owns the reference.
    // It increases the object's reference count and the caller is responsible for eventually
    // calling `Py_DECREF` to release it.
    //
    // 2) 'Borrowed' reference is a pointer to an object that does _not_ own the reference.
    // It does _not_ increase the object's reference count. The caller presumes that
    // the referenced object already exists and has a positive refcount.
    // The caller should not call `Py_DECREF` on the object (no need to release).
    // It can become a dangling pointer if the object is destroyed.
    //
    // See section "Reference Count Details" in the Python/C API Reference Manual
    // <https://docs.python.org/3/c-api/intro.html#reference-count-details>
    bool m_borrowed{false};
};


//! Wrapper for a Numpy array to be passed to C++. The class stores the basic information
//! which is required to define a Numpy array: size, rank and shape of the array and its
//! datatype. The C++ code can then access the array data via a raw pointer.
//! The accompanying Python object is also internally managed by the class.
//! NOTE: The array data is not copied.
class NumpyArrayWrapper {

public:
    NumpyArrayWrapper(const std::size_t datasize, const std::size_t n_dims, const std::size_t* dims,
                      const std::string& dtype, const void* array_ptr, PyObject* py_array_ptr,
                      const ArrayMetadata& metadata = {});

    ~NumpyArrayWrapper();

    //! Total size of the array
    std::size_t size() const { return m_datasize; }

    //! Rank, number of dimensions of the array
    std::size_t rank() const { return m_dims.size(); }

    //! Raw pointer to the array which stores the size of the underlying C-Array on each dimension
    const std::size_t* dimensions() const { return m_dims.data(); }

    //! Raw pointer to the underlying C-array
    const void* arrayPtr() const { return m_array_ptr; }

    //! Data-type ('dtype') of the array
    const std::string dtype() const { return m_dtype; }

    //! ArrayMetadata of the array
    const ArrayMetadata& metadata() const { return m_metadata; }

private:
    std::size_t m_datasize{0};         //!< total size of the array
    std::vector<std::size_t> m_dims;   //!< dimensions of the arrary
    std::string m_dtype;               //!< data-type ('dtype') of the array
    ArrayMetadata m_metadata;          //!< metadata of the array
    const void* m_array_ptr{nullptr};  //!< raw pointer to the array data
    PyObject* m_py_array_ptr{nullptr}; //!< raw pointer to the Numpy array (Python object)
};

#endif // BORNAGAIN_PYCORE_EMBED_PYOBJECTPTR_H
