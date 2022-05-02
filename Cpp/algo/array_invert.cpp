#include <iostream>
#include <vector>

using namespace std;

vector<string> split_string(string);

vector<int> reverseArray(vector<int>& a) {
  const size_t N = a.size(), iMax = N - 1, N_half = N / 2;
  cout << "N_half = " << N_half << '\n';
  for (size_t i = 0; i < N_half; ++i) {
    // swap a[i] and a[N-i-1]
    printf("%lu) %d -- %d\n", i, a[i], a[iMax - i]);
    const int v = a[i];
    a[i] = a[iMax - i];
    a[iMax - i] = v;
  }
  return a;
}

int main()
{
  
  vector<int> arr {1, 2, 3, 4};

  vector<int> res = reverseArray(arr);
  cout << ".....\n";

  for (int i = 0; i < res.size(); ++i) {
    cout << res[i] << ",";
  }

  cout << "\n";

   return 0;
}
