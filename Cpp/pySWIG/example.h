/* File: example.h */

#include <memory>

typedef double value_t;

struct FooArray {
    int size;
    value_t* array_ptr;
};

class Foo
{
public:   
    int size = 0;
    value_t* array_ptr = nullptr;
    
    Foo();
    ~Foo();
    Foo(const int sz);
    
    void say() const;
    void store(Foo* foo_ptr);
    // neeed for interfacing C-arrays
    struct FooArray array();
    // void array(int* sz, value_t** a_ptr);

#ifndef SWIG
    std::unique_ptr<Foo> clone();
#endif
private:
    void _say();
    std::unique_ptr<Foo> _ptr;
};

long int fact(int n);
