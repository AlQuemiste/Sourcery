// find the size of a char array
#include <cinttypes>
#include <stdio.h>

#define SIZEOF(a) ( sizeof(a) / sizeof(a[0]) )

std::uint32_t H = 1;

unsigned int strsize(char* const str) {
  unsigned int s = sizeof(str) / sizeof(str[0]);
  return s;
}

int main(void) {
  char str [] = "abcde";
  // inline
  int size1 = sizeof(str) / sizeof(str[0]);
  // function call
  int size2 = strsize(str);

  printf("* size: expected  %d\n", 6);
  char ch = str[0];
  std::uint8_t i = 0;
  while (ch != '\0') {
    printf("%d) %c\n", i, ch);
    i += 1u;
    ch = str[i];
  }
  printf("  inline)         %d\n", size1);
  printf("  function call)  %d\n", size2);
  printf("  macro)  %d\n", SIZEOF(str));

  return 0;
}


// compile>>> gcc -o strsize.out strsize.c
