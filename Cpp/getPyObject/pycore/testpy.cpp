#include "pycore.hpp"
#include <iostream>

int main() {
    // prepare arguments for the function call
    const double x = 3.14, y = 2.0;
    const long int b = -42;
    const char py_func_name[] = "real_fn";
    const char py_module_name[] = "my_module";
    double result;

    std::cout << "> Calling Python function '" << py_func_name << "' "
              << "in module '" << py_module_name << "' "
              << "with x = " << x << ", y = " << y << ", b = " << b
              << std::endl;

    const int status = runFunction(py_func_name, py_module_name,
                                   x, y, b, result);

    std::cout << ">> returned status = " << status << std::endl;

    return status;
}
