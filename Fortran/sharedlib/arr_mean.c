#include <stdio.h>

extern void calculate_mean(const double* A, double* B, const int n, int* m);

int main()
{
    int n = 5;
    double A[] = {1.0, 2.0, 3.0, 4.0, 5.0};
    double B[n];
    int m = 0;

    calculate_mean(A, B, n, &m);

    for (int i = 0; i < n; i++) {
        printf("B[%d] = %f\n", i, B[i]);
    }

    printf("m = %d\n", m);

    return 0;
}

// gcc -o arr_mean.exe arr_mean.c -L. -larr_mean -Wl,-rpath,.
