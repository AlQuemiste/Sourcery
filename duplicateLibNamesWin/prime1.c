__declspec(dllexport) int isPrime1(const int n);

int isPrime1(const int n)
{
    if (n == 1) {
        return -1;
    }

    int flag = 0;
    for (int i = 2; i <= n / 2; ++i) {
        // condition for non-prime
        if (n % i == 0) {
            flag = 1;
            break;
        }
    }

    return flag;
}
