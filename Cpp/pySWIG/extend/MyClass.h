class MyClass {
public:
    int i = 0;
    // Python function
    void* pyFunc = nullptr;

    // externally defined function which prints the Python function name
    void (*pyGetName)(void*) = nullptr;

    void name();

#ifndef SWIG
    void configure(void* pFunc_, const int i_);

    ~MyClass();
#endif // SWIG

};

struct Info {
    int i = 0;
    double x = 0;
};
