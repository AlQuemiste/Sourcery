#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

bool increment(size_t* const counter, const size_t vmax) {
  bool carry = false;
  for(size_t i = 0; i < vmax; ++i) {
    counter[i] += 1;
    if (counter[i] > vmax) {
      counter[i] = 0;
      carry = true;
    }
    if (!carry) break;
  }

  return (counter[vmax - 1] <= vmax);
}

void intPermutations(const size_t vmax) {
  // vmax must be > 0
  if (vmax < 1)
    return;

  // initialize the permutations
  size_t* counter = (size_t*) malloc(vmax * sizeof(size_t));
  for (size_t i = 0; i < vmax; ++i) {
    counter[i] = i;
  }

  bool exists = true;
  int _c = 0;
  while (exists && _c < 15) {
    for (size_t i = 0; i < vmax; ++i) {
      printf("%lu ", counter[i]);
    }
    printf("\n");

    exists = increment(counter, vmax);
    _c += 1;
  }

  free(counter);
  return;
}


int main() {

  const size_t vmax = 3;
  intPermutations(vmax);

  return 0;
}
