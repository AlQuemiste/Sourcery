#include <stdlib.h>
#include <stdio.h>

typedef struct rec
{
  int i;
  float PI;
  char A;
} record;

int foo(int x, const int y) {
  return x + y;
}

int main()
{

  record* ptr_3 = (record*) malloc(sizeof(record) * 3);

  record* ptr_one = ptr_3 + 1;
  ptr_one->i = 10;
  (*ptr_one).PI = 3.14;
  (*ptr_one).A = 'a';

  printf("First value: %d\n",ptr_one->i);
  printf("Second value: %f\n", ptr_one->PI);
  printf("Third value: %c\n", ptr_one->A);
  printf("sizeof pointer 1: %d\n", sizeof(ptr_one));
  printf("sizeof pointer 3: %d\n", sizeof(ptr_3));

  int (*f_ptr)(int, int) = foo;
  
  free(ptr_3);
  printf("sizeof pointer 3: %d\n", sizeof(ptr_3));
  printf("sizeof foo pointer: %d\n", sizeof(f_ptr));

  return 0;
}
