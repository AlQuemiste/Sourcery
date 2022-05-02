#include <iostream>

enum StateFlag {Failure, Success};

using namespace std;

class Queue {
public:
  const size_t size = 0;
  size_t begin = 0;
  size_t end = 0;
  float** que = nullptr;

  Queue(const size_t size_): size(size_) {
    if (size > 0) {
      que = new float* [size];
      for (int i = 0; i < size; ++i)
        que[i] = nullptr;
    }

  }

  ~Queue() {
    if (que) {
      delete [] que;
      printf("* queue storage deallocated.\n");
    }
  }

public:

  StateFlag enqueue(float* x) {
    float* p = que[end];
    if (p) return Failure;
    que[end] = x;
    end += 1;
    end = end == size? 0 : end;
    return Success;
  }

  float* dequeue() {
    float* p = que[begin];
    if (!p) return p;
    que[begin] = nullptr;
    begin += 1;
    begin = begin == size? 0 : begin;
    return p;
  }

};

int main() {
  float nums[] = {1, 2, 3, 4, 5};
  const size_t N = sizeof(nums) / sizeof(nums[0]);
  Queue Q0(N);

  for (int i = 0; i < N; ++i) {
    StateFlag success = Q0.enqueue(&nums[i]);
    printf("%i) Queue begin = %d, end = %d <done: %d>\n", i, Q0.begin, Q0.end, success);
  }
  printf("* enqeue done.\n");

  for (int i = 0; i < N - 2; ++i) {
    float* f_ptr = Q0.dequeue();
    if (f_ptr)
      printf("%d) Queue begin = %d, end = %d => %f\n", i, Q0.begin, Q0.end, *f_ptr);
    else
      printf("%i) Queue begin = %d, end = %d => FAILED\n", i, Q0.begin, Q0.end);
  }
  printf("* deqeue done.\n");

  printf("*** stage II ***\n");

  for (int i = 0; i < N; ++i) {
    StateFlag success = Q0.enqueue(&nums[i]);
    printf("%i) Queue begin = %d, end = %d <done: %d>\n", i, Q0.begin, Q0.end, success);
  }
  printf("* enqeue done.\n");

  for (int i = 0; i < N - 2; ++i) {
    float* f_ptr = Q0.dequeue();
    if (f_ptr)
      printf("%d) Queue begin = %d, end = %d => %f\n", i, Q0.begin, Q0.end, *f_ptr);
    else
      printf("%i) Queue begin = %d, end = %d => FAILED\n", i, Q0.begin, Q0.end);
  }
  printf("* deqeue done.\n");
  return 0;
}
