#include "libfoo.hpp"

#include <iostream>
#include <string>
#include <vector>

class Foo
{
public:
    int i = 0;
    double x = 0;

    Foo(int i, double x):
        i{i}, x{x}
        {
            std::cout << "libfoo.Foo: init with i = " << i << ", x = " << x << std::endl;
        }

    std::string str()
        {
            std::string rpr = "libfoo.Foo: i = " + std::to_string(i) + ", x = " + std::to_string(x);
            return rpr;
        }
};


std::string getString(const char* const str_in)
{
    std::string str_out = str_in;
    str_out += " -- libfoo";
    std::cout << "libfoo.getString: input = '" << str_in
              << "', output = '" << str_out << "'" << std::endl;
    return str_out;
}

std::string getStringStr(const std::string str_in)
{
    std::string str_out = str_in;
    str_out += " -- libfoo";
    std::cout << "libfoo.getStringStr: input = '" << str_in
              << "', output = '" << str_out << "'" << std::endl;
    return str_out;
}

Array<double> getArrayF64(const double* const array, const int dims, const int* shape)
{
    Array<double> arr;
    arr.dims = dims;
    arr.shape.resize(dims);
    arr.size = 1;
    std::cout << "libfoo.getArrayF64: dims = " << dims << ", shape = (";
    for (int d = 0; d < dims; ++d)
    {
        arr.shape[d] = shape[d];
        arr.size *= shape[d];
        std::cout << d << ", ";
    }
    std::cout << ")" << std::endl;
    arr.data.resize(arr.size);
    for (int idx = 0; idx < arr.size; ++idx)
    {
        arr.data[idx] = array[idx];
    }

    return arr;
}

int foo(const int i, int j)
{
    return i + j;
}

void qux(const int i, int& j)
{
    j = i + 2;
}
