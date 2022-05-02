// Default argument to virtual function

#include <iostream>
using namespace std;

class A
{
    public:
        virtual void stuff(int i = 5) { cout << "A" << i << endl; }
};

class B : public A
{
    public:
        virtual void stuff(int i = 10) { cout << "B" << i << endl; }
};

int main(int argc, char **argv)
{
    A *a = new B();
    a->stuff();
    return 0;
}
