#include <iostream>

enum Switch {On, Off};

int qux0(const int num) {
    int t = num + 1;
    return t * num;
}

int foo0(const int num) {
    constexpr Switch i = On;

    int t = -1;
    switch (i) {
        case On:
            t = num + 1;
           break;
        case Off:
            t = 1;
	    break;
        default:
            t = 0;
	    break;
    }

    return t * num;
}

inline int bar0(const int num, const Switch sw) {

    int t = -1;
    switch (sw) {
        case On:
            t = num + 1;
           break;
        case Off:
            t = 1;
	    break;
        default:
            t = 0;
	    break;
    }

    return t;
}

int baz0(const int num) {
  const int t = bar0(num, On);
  return t * num;
  
}

int qux1(const int num) {
  int t = 0;
  for (int i = 0; i < 3; ++i)
    t += num;

  return t;  
}

inline int bar1(const int num, const Switch s) {
  int t = 0;
  for (int i = 0; i < 3; ++i)
    switch (s) {
    case On:
      t += num;
      break;
    case Off:
      t -= num;
      break;
    }

  return t;
}


int baz1(const int num) {
  int t = bar1(num, On);
  return t;
}
  

int main(int argc, char**argv) {
  int num;
  if (argc > 1)
    num = atoi(argv[1]);
  else
    num = 10;

  int i = foo0(num);
  int j = baz0(num);

  printf(">> num = %d; i = %d, j= %d\n", num, i, j);

  return i + j;
}


/*

Test the generated assembly code by:
$ gcc -S -masm=intel -O1 constexpr_test.cpp

Compare the resulting machine code for qux and baz functions;
they must be the same. 
 */
