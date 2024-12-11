#include "pycore.hpp"
#include <iostream>

int main() {
    initializePython();

    // Python path
    std::cout << "> Python path:\n'''\n" << PythonPath() << "\n'''" << std::endl;

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

    // test importing numpy
    std::cout << "> Importing numpy" << std::endl;
    const int np_status = import_numpy();
    std::cout << ">> returned status = " << np_status << std::endl;

    finalizePython();

    return status;
}
