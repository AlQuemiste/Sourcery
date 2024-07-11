"""
test example library
"""
import ctypes
import numpy as np
import math

import example as exm

#----------------------------------------------

n0 = 11
fact_n0 = exm.fact(n0)

print("# Test example library")
print()

print("## Test fact()")
print("%d! = %d" % (n0, fact_n0), "=?=", math.factorial(n0))
print()

print("## Test class Foo")
foo_ = exm.Foo()
foo_n0 = exm.Foo(n0)
foo_n0.say()
print()

print("## Test FooArray")

value_t = ctypes.c_double
foo_array = foo_n0.array()
print("FooArray: size = %d, a_ptr = %s" %(foo_array.size, foo_array.array_ptr))

py_array_ptr = (value_t * foo_array.size).from_address(int(foo_array.array_ptr))

# as a list

print("convert to Python list:")
print(list(py_array_ptr))

np_array = np.ctypeslib.as_array(py_array_ptr, shape=(foo_array.size,))
print("convert to Numpy array:")
print(np_array)
# see <https://stackoverflow.com/questions/51776809/read-swig-python-double-object-into-numpy-maybe-through-ctypes>
print()

del foo_n0
print("# END test.")
