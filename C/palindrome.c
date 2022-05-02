#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(){
    char str[20];

    printf("Enter a string: ");
    scanf("%s", str);
    const size_t ln = strlen(str),
      ln_2 = ln / 2;

    unsigned int flag = 1;
    for (int i = 0; i <= ln_2; ++i)
      {
        if (tolower(str[i]) != tolower(str[ln - 1 - i])) {
          flag = 0;
          break;
        }
      }

    if (flag)
      printf("'%s' is a palindrome", str);
    else
      printf("'%s' is NOT a palindrome", str);

    printf("\n");
    return 0;
}
