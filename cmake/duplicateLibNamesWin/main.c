#include <stdio.h>

int isPrime1(const int n);

int main(void)
{
  puts("This is main");
  int r = isPrime1(2);
  printf("isPrime(2) = %d", r);

  return 0;
}
