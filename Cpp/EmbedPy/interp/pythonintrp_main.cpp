#include <iostream>
#include "PythonInterpreter.h"

#include <string>

int main()
{

    using std::cout, std::endl;
    using Python = PythonInterpreter;
    using Numpy = Python::Numpy;
    using Fabio = Python::Fabio;

    cout << "--- BEGIN ---" << endl;

    Numpy::initialize();
    Status st0 {PythonInterpreter::checkError()};
    if(!st0)
        cout << st0.message << endl;

    { // automatically release the used Python memory after end of this block scope
    PyObjectPtr fabio_module {Python::import("fabio")};
    if(!fabio_module.status)
        cout << fabio_module.status.message << endl;

    PyObjectPtr npy_array {Fabio::open("marbles.tif", fabio_module)};
    if(!npy_array.status)
        cout << npy_array.status.message << endl;

    double* c_array = Numpy::NpyArray_to_C(npy_array);

    // npy_array.discard();
    // fabio_module.discard();
    }

    st0 = PythonInterpreter::finalize();
    if(!st0)
        cout << st0.message << endl;
    else
        cout << "PythonInterpreter finalized" << endl;

    cout << "--- END ---" << endl;
    return 0;
}
