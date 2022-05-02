/* File: example.c */

#include "example.h"
#include <iostream>

int fact(int n) {
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

Foo::~Foo()
{
    std::cout << "Foo<" << this << "> destroy" << std::endl;
}

void Foo::say()
{
    std::cout << "Hello I am Foo<" << this << ">" << std::endl;
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
