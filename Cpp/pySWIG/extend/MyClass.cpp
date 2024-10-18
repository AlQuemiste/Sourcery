#include "MyClass.h"
#include <iostream>

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

MyClass::~MyClass()
{
    std::cout << "C++ MyClass<" << this << "> destroyed." << std::endl;
}
