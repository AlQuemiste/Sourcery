#include <string>
#include <iostream>

using PyObject = int;

class PyObjectPtr
{
public:
    enum class ResultType {None=-1, Info, Warning, Error};

    PyObject* ptr = nullptr;
    ResultType resultType = ResultType::Info;
    std::string message;

    PyObjectPtr(PyObject* pyobject_ptr,
                const ResultType result_type, const std::string& message):
        ptr{pyobject_ptr},
        resultType{result_type},
        message{message}
        {
            // if PyObject pointer is null but result type is not `None` or `Error`,
            // then that will be an `Error` case.
            if(!ptr && result_type != ResultType::None && result_type != ResultType::Error)
            {
                resultType = ResultType::Error;
            }

            std::cout << "PyObjectPtr: Create PyObjectPtr" << std::endl;
        }

    PyObjectPtr(PyObject* pyobject_ptr, const std::string& message = ""):
        PyObjectPtr(pyobject_ptr, ResultType::Info, message)
        {};

    PyObjectPtr(const ResultType result_type, const std::string& message = ""):
        PyObjectPtr(nullptr, result_type, message)
        {};

    ~PyObjectPtr()
        {
            release();
        }

    // disallow copy constructor
    PyObjectPtr(const PyObjectPtr&) = delete;
    // disallow copy assignment
    PyObjectPtr& operator=(const PyObjectPtr&) = delete;
    // allow move constructor
    PyObjectPtr(PyObjectPtr&& other):
        PyObjectPtr(other.ptr, other.resultType, other.message)
        {
            other.reset();
            std::cout << "MOVE constructor for " << message << std::endl;
        }

    void reset()
        {
            ptr = nullptr;
            resultType = ResultType::None;
            message = "";
        }

    void release()
        {
            if(ptr) {
                std::cout << "PyObjectPtr: Released PyObjectPtr" << std::endl;
                ptr = nullptr;
            }
        }

    bool valid() const
        {
            return (ptr && (resultType == ResultType::Info || resultType == ResultType::Warning));
        }
};

//========================================

PyObjectPtr foo()
{
    int i = 10;
    return {&i, PyObjectPtr::ResultType::Info, "<Foo Result>"};
}


int main()
{
    using std::cout, std::endl;
    cout << std::boolalpha;

    PyObjectPtr p1(PyObjectPtr::ResultType::Error, "Not Valid");
    cout << "p1 = " << p1.valid() << endl;

    PyObjectPtr p2(nullptr);
    cout << "p2 = " << p2.valid() << endl;

    PyObjectPtr p3(nullptr, PyObjectPtr::ResultType::Info, "");
    cout << "p3 = " << p3.valid() << endl;
    cout << "p3.resultType = " << static_cast<int>(p3.resultType) << endl;

    PyObjectPtr p4{foo()};
    cout << "p4 = " << p4.valid() << "; message = '" << p4.message << "'" << endl;

    PyObjectPtr p5(std::move(p4));
    cout << "p5 = " << p5.valid() << "; message = '" << p5.message << "'" << endl;
    cout << "p4 = " << p4.valid() << "; message = '" << p4.message << "'" << endl;

    return 0;
}
