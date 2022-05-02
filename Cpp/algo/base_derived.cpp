#include <iostream>
using std::cout;

class Base
{
public:
  int x = 1;
  
  Base() {
    cout << "Base class constructed\n";
  }

  virtual ~Base() {
    cout << "Base class destructed\n";
  }

  virtual void foo() {
    cout << "Base class foo: x = " << x << '\n';
  }
};

class Derived: public Base
{
public:
  int x = 2;
  
  Derived() {
    cout << "Derived class constructed\n";
  }

  ~Derived() {
    cout << "Derived class destructed\n";
  }

  void foo() {
    cout << "Derived class foo: x = " << x << '\n';
  }
};


int main() {

  Base* ptr = new Derived();
  ptr->foo();
  delete ptr;
  return 0;
}
