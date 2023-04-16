#include <string>
#include <vector>

int foo(const int i, int j);

void qux(const int i, int& j);

std::string getString(const char* const str_in);

std::string getStringStr(const std::string str_in);

template<typename NumType>
struct Array
{
    int dims = 0;
    int size = 0;
    std::vector<int> shape;
    std::vector<NumType> data;
};

Array<double> getArrayF64(const double* const array, const int dims, const int* shape);
