class Foo;

class MyClass {
public:
    int i = 0;
    // Python function
    void* pyFunc = nullptr;

    MyClass();

    // externally defined function which prints the Python function name
    void (*pyGetName)(void*) = nullptr;

    void name();

    int calcFoo(Foo& foo, const int i);

#ifndef SWIG
    void configure(void* pFunc_, const int i_);

    ~MyClass();
#endif // SWIG

};

struct Info {
    int i = 0;
    double x = 0;
};

class Foo {
public:
    Foo();
    Foo(const int i_);
    virtual ~Foo();

    virtual int calc(const int j);

private:
    const int i;
};
