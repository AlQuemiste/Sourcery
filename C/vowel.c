#include <stdio.h>
#include <ctype.h>

int main()
{
  char a;
  unsigned char isVowel = 0;
  printf("Enter a char:\t");
  scanf("%c",&a);

  switch (tolower(a))
    {
    case 'a':
      isVowel = 1;
      break;
    default:
      break;
    }

  if (isVowel)
    printf("\n%c is a vowel", a);
  else
    printf("\n%c is not a vowel",a);

  printf("\n");
  
  
  /*   if(a=='A' || a=='a' || a=='E' || a=='e' || a=='I' || a=='i' || a=='O' || a=='o' || a=='U' || a=='u') */
  /*     { */
  /*       printf("\n%c is a vowel",a);	 */
  /*     } */
  /*   else */
  /*     printf("\n%c is not a vowel",a); */
  /* } */

  return 0;
}
