#include <iostream>
#include <vector>

void print(int x)
{
  std::cout << "print(int): " << x << '\n';
}
 
void print(int *x)
{
  if (!x)
    std::cout << "print(int*): null\n";
  else
    std::cout << "print(int*): " << *x << '\n';
}

struct A {
  int i = 0;
  int j = -1;
};
 
int main()
{
  
  int *x { nullptr };
  print(x); // calls print(int*)
 
  print(nullptr); // calls print(int*) as desired

  const std::vector<int> v = {1, 2, 3, 4};
  std::cout << v[1] << std::endl;
  // v[2] = 6;

  // vector of constant objects
  const A a0 = {.i = -1, .j = -2};
  const A b0;
  std::cout << b0.i << std::endl;

  const std::vector<A> w = {A{1,2}, a0, A{4,5}};
  // w[1] = a0;
  // a0.j += -5;
  std::cout << a0.j << std::endl;
  std::cout << w[1].j << std::endl;
  // w[1].j = -4;
  w.push_back(A{4,4});
  return 0;
}
