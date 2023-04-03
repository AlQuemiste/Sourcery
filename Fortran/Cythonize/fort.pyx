from numpy cimport ndarray
from numpy import empty

cdef extern:
    void c_mesh_exp(double *r_min, double *r_max, double *a, int *N)

def mesh_exp(double r_min, double r_max, double a, int N):
    # cdef ndarray[double, mode="c"] mesh = empty(N, dtype=double)
    c_mesh_exp(&r_min, &r_max, &a, &N)
    return 1
    
def myfoo(double r):
    return r+1
