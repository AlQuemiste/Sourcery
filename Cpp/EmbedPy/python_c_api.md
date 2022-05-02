# Notes on Python C/API

## Numpy

* PY_ARRAY_UNIQUE_SYMBOL
https://github.com/numpy/numpy/issues/9309

* Py_IsInitialized()

* Py_InitializeEx(0)
------------------------------

* Python::toString
Sample/Multilayer/PyImport.cpp
Tests/Functional/PyEmbedded/Tests.cpp:69:

* Python::toString::toVectorString
Base/Py/PyUtils.cpp

* Python::import_bornagain
Sample/Multilayer/PyImport.cpp

* Python::pythonRuntimeInfo
Tests/Functional/PyEmbedded/Tests.cpp:49:

* Python::pythonStackTrace
Sample/Multilayer/PyImport.cpp

* Python::createNumpyArray
  bornagain/Device/Histo/Histogram1D.cpp

-------------------

* BORNAGAIN_PYTHON
Base/Py/PyCore.h
Base/Py/PyUtils.cpp
Base/Py/PyUtils.h
Base/Py/PyObject.h
Sample/Multilayer/PyImport.h
Sample/Multilayer/PyImport.cpp
GUI/Model/From/SampleListModel.h GUI/Model/From/SampleListModel.cpp
GUI/View/SampleDesigner/SampleListView.h
GUI/View/SampleDesigner/SampleListView.cpGUI/View/Project/PyImportAssistant.cpp
Device/Data/OutputData.cpp
Device/Histo/SimulationResult.h
Device/Histo/SimulationResult.cpp

* BORNAGAIN_PYTHONAPI_ARRAY
auto/Wrap/libBornAgainCore_wrap.cpp
auto/Wrap/libBornAgainFit_wrap.cpp
auto/Wrap/libBornAgainParam_wrap.cpp
auto/Wrap/libBornAgainSample_wrap.cpp
auto/Wrap/libBornAgainBase_wrap.cpp
auto/Wrap/libBornAgainDevice_wrap.cpp

* import_array
auto/Wrap/libBornAgainCore_wrap.cpp
auto/Wrap/libBornAgainFit_wrap.cpp
auto/Wrap/libBornAgainResample_wrap.cpp
auto/Wrap/libBornAgainParam_wrap.cpp
auto/Wrap/libBornAgainSample_wrap.cpp
auto/Wrap/libBornAgainBase_wrap.cpp
auto/Wrap/libBornAgainDevice_wrap.cpp

----------------------------------------

* PyObject used in

Device/Data/OutputData.h, .cpp
Device/Histo/Histogram1D.h, .cpp
Device/Histo/SimulationResult.h, .cpp
Tests/Functional/PyEmbedded/Tests.cpp

----------------------------------------

* CPython PyObject:
  ```
  /* Nothing is actually declared to be a PyObject, but every pointer to
  * a Python object can be cast to a PyObject*.  This is inheritance built
  * by hand.  Similarly every pointer to a variable-size Python object can,
  * in addition, be cast to PyVarObject*.
  */
  typedef struct _object {
    _PyObject_HEAD_EXTRA
    Py_ssize_t ob_refcnt;
    PyTypeObject *ob_type;
    } PyObject;
  ```
  <https://github.com/python/cpython/blob/main/Include/object.h>

  - <https://numpy.org/doc/stable/reference/c-api/types-and-structures.html>

* Python's Garbage Collector
https://rushter.com/blog/python-garbage-collector

----------------------------------

Python objects are structures allocated on the heap. Accessed through pointers of type PyObject*

----------------------------------
Python Extension Patterns
https://pythonextensionpatterns.readthedocs.io/en/latest/refcount.html
