# ref: <https://codereview.stackexchange.com/q/92266>

def array_tutorial(a):
    print("a.shape={}, a.dtype={}".format(a.shape, a.dtype))
    print(a)
    a *= 2
    return a[-1]
