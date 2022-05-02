#include <stdio.h>

int main(void)
{
    int i;
    int j;

    char* name = "The Name";

    goto finally;

    i = 1; j = -2;
    int k = 2;
    name = "The Name2";

finally:
    printf("finalizing %d...\n", __LINE__);

    return 0;
}
