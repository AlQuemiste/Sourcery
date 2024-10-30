#include "MyClass.h"
#include <iostream>

MyClass::MyClass()
{
    std::cout << "C++ MyClass<" << this << "> constructed." << std::endl;
}

void MyClass::name()
{
    if (pyGetName) {
        std::cout << "C++ MyClass<" << this << ">.name:" << std::endl;
        pyGetName(this->pyFunc);
    }
}

void MyClass::configure(void* pFunc_, const int i_)
{
    i = i_;
    pyFunc = pFunc_;
    std::cout << "C++ MyClass<" << this << "> configured." << std::endl;
}

int MyClass::calcFoo(Foo& foo, const int i)
{
    return foo.calc(i);
}

MyClass::~MyClass()
{
    std::cout << "C++ MyClass<" << this << "> destroyed." << std::endl;
}

Foo::Foo():
    i{-1}
{
    std::cout << "C++ Foo<" << this << "> created with default i = " << i << std::endl;
}

Foo::Foo(const int i_):
    i{i_}
{
    std::cout << "C++ Foo<" << this << "> created with i = " << i << std::endl;
}

Foo::~Foo()
{
    std::cout << "C++ Foo<" << this << "> destroyed." << std::endl;
}

int Foo::calc(const int j)
{
    return i + j;
}
