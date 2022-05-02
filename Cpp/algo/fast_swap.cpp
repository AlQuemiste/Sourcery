// Complete the hourglassSum function below.
#include <iostream>

template <typename Num_t>
void swap(Num_t& x, Num_t& y) {
  x = x ^ y;
  y = y ^ x;
  x = x ^ y;
}

int main()
{
  int a = -1;
  int b = 2;
  swap(a, b);
  std::cout << "a -> " << a << ", b -> " << b << '\n';
  return 0;
}
