#include <functional>
#include <iostream>

using std::cout, std::endl;

typedef int (*fn_t) (const int);

void caller(fn_t fn, int i) {
  int j = fn(i);
  cout << "fn(" << i << ") = " << j << endl;
}

template <class T>
using m_fn = int (T::*) (const int);

template<class T>
void m_caller(m_fn<T> fn, T* t, int i) {
  int j = (t->*fn)(i);
  cout << "m_fn(" << i << ") = " << j << endl;
}

int foo(const int m) {
  cout << "foo called with m = " << m << endl;
  return m + 2;
}


struct A {
  int bar(const int m) {
    cout << "A::bar called with m = " << m << endl;
    return m - 1;
  }

};

int main() {
  fn_t foo_ptr = foo;
  caller(foo_ptr, 2);

  caller(foo, 3);

  A a;
  m_caller<A>(&A::bar, &a, 4);
  return 0;
}
