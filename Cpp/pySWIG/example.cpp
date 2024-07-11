/* File: example.c */

#include "example.h"
#include <iostream>

long int fact(int n) {
    if (n < 0){ /* This should probably return an error, but this is simpler */
        return 0;
    }
    if (n == 0) {
        return 1;
    }
    else {
        /* testing for overflow would be a good idea here */
        return n * fact(n-1);
    }
}


Foo::Foo()
{
    std::cout << "Foo<" << this << "> create" << std::endl;
}

Foo::Foo(const int sz): size{sz}
{
    array_ptr = new value_t[size];
    for (int i = 0; i < size; ++i) {
        array_ptr[i] = i + 1;
    }
}

Foo::~Foo()
{
    if (array_ptr) {
        std::cout << "Foo<" << this << "> destroy array[" << size << "]" << std::endl;
        delete[] array_ptr;
    }
    
    std::cout << "Foo<" << this << "> destroy" << std::endl;
}

void Foo::say() const
{
    std::cout << "Hello I am Foo<" << this << "> of size " << size << std::endl;
}

struct FooArray Foo::array()
{
    return {size, array_ptr};
}


#ifndef SWIG
std::unique_ptr<Foo> Foo::clone()
{
    std::cout << "Cloning Foo<" << this << ">" << std::endl;
    return std::make_unique<Foo>();
}
#endif

void Foo::_say()
{
    std::cout << "Hello I am Foo<" << this << ">" << std::endl;
}


void Foo::store(Foo* foo_ptr)
{
    std::cout << "Foo <" << this << "> store <" << foo_ptr << ">" << std::endl;
    _ptr.reset(foo_ptr);
}
