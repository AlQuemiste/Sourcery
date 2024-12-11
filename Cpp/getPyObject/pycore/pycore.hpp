#include <string>

void initializePython();

void finalizePython();

int callPyFunction(void* pyFunc,
                   const double x, const double y, const long int b,
                   double& result);

int runFunction(const char py_func_name[], const char py_module_name[],
                const double x, const double y, const long int b,
                double& result);

int import_numpy();

std::string PythonPath();
