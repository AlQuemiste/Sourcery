import ctypes
import numpy as np

# load the shared library
xlib = ctypes.CDLL('./libarr_mean.so')

# define the argument and return types for the external function
xlib.calculate_mean.argtypes = [
    np.ctypeslib.ndpointer(dtype=np.float64, ndim=1, flags='C_CONTIGUOUS'),
    np.ctypeslib.ndpointer(dtype=np.float64, ndim=1, flags='C_CONTIGUOUS'),
    ctypes.c_int,
]
xlib.calculate_mean.restype = None

# create sample input array
n = 5
A = np.array([1, 2, 3, 4, 5], dtype=np.float64)
# create an output array
B = np.empty(n, dtype=np.float64)

# call the external function to calculate the mean
xlib.calculate_mean(A, B, n)

# print the results
print("size(a) = %d, mean(a) = %f" % (A.size, A.mean()))
for i in range(n):
    print(f"B[{i}] = {B[i]}")
