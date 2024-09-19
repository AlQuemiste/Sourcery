import numpy as np
import ctypes

def ArrayConverter(np_array):
    arr_ctype = np.ctypeslib.as_ctypes_type(np_array.dtype)
    array_ptr_t = ctypes.POINTER(arr_ctype)
    # get the pointer to the raw array data
    arr_ptr = np_array.ctypes.data_as(array_ptr_t)
    arr_void_ptr = ctypes.cast(arr_ptr, ctypes.c_void_p)
    # get the size and the dimensions of the array
    arr_size = np_array.size
    arr_dims = tuple(np_array.shape)
    arr_ndims = len(arr_dims)
    arr_dtype = np_array.dtype.name

    print("Python: arr_size = %d, arr_ndims = %d, arr_dims = %s, dtype = '%s'"
          % (arr_size, arr_ndims, arr_dims, arr_dtype))

    return (arr_void_ptr.value, arr_size, arr_ndims, arr_dims, arr_dtype)


def fn():
    array_t = ctypes.c_int
    array_ptr_t = ctypes.POINTER(array_t)
    void_ptr = ctypes.c_void_p

    # create a NumPy array
    arr = np.array([[1, 2], [3, 4], [5, 6]], dtype=array_t)

    return ArrayConverter(arr)


def fn_array():
    array_t = ctypes.c_int
    array_ptr_t = ctypes.POINTER(array_t)
    void_ptr = ctypes.c_void_p

    # create a NumPy array
    arr = np.array([[1, 2], [3, 4], [5, 6]], dtype=array_t)

    return arr


def real_fn(x:float, y:float, b:int) -> float:
    res = x * y + b
    print("Python: real_fn called with x = %f, y = %f, b = %d and result = %f"
          % (x, y, b, res))
    return res
