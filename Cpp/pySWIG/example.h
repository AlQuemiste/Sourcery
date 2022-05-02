/* File: example.h */

#include <memory>

class Foo
{
public:
    Foo();
    ~Foo();
    void say();

    void store(Foo* foo_ptr);

#ifndef SWIG
    std::unique_ptr<Foo> clone();
#endif
private:
    void _say();
    std::unique_ptr<Foo> _ptr;
};

int fact(int n);
