from libcpp.vector cimport vector
from libcpp.string cimport string

import sys
from importlib import import_module


cdef extern from "libfoo.hpp":
    int foo(const int i, int j)
    void qux(const int i, int& j)
    string getString(const char* const str_in)
    string getStringStr(const string str_in)

def Foo(i, j):
    print("py.Foo: i = {}, j = {}".format(i, j))
    s = foo(i, j)
    return s

def Qux(i, j):
    print("py.Qux: i = {}, j = {}".format(i, j))
    cdef int jj = j
    cdef int* j_ptr = &jj
    qux(i, jj)
    print("py.Qux: j => {}".format(jj))

def String(str_):
    print("py.String: str_ = {}".format(str_))
    str_out = getString(str_)
    return str_out

def StringStr(str_):
    print("py.StringStr: str_ = {}".format(str_))
    str_out = getStringStr(str_)
    return str_out

def ModuleObjects(module_name, module_path=""):
    if module_path:
        sys.path.append(module_path)
    module = import_module(module_name)
    module_objects = [obj for obj in dir(module) if not obj.startswith('_')]
    return module_objects
