#include <iostream>

using std::cout;
using std::endl;

class Base
{
public:
  int i = 0;
  Base(int i_): i(i_) {
    cout << "Base::init: i = " << i << endl;
  }
  
  void foo() {
    cout << "Base::foo: i = " << i << endl;
  }
};

class B: public Base
{
public:
  int k = 2;
  int j = 3;
  B(): Base(1) {}
  
  void bar() {
    cout << "B::bar: k = " << k << endl;
  }

};

class A: public Base
{
public:
  int j = 1;
  A(): Base(2) {}

  void qux() {
    cout << "A::qux: j = " << j << endl;
  }

};
 
class Derived: public A, public B
{
public:
  void zoo()
  {
    cout << "Derived::zoo: k = " << k << endl;
  }
};


int main() {
  Derived d;
  d.zoo();
  cout << "k = " << d.k << endl;
  return 0;
}
