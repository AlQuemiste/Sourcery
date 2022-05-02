#include <iostream>

#define SUCCESS 1
#define FAILURE 0

using namespace std;

struct Queue {
  const size_t size = 0;
  size_t begin = 0;
  size_t end = 0;
  float** que = nullptr;
};

Queue mk_queue(const size_t size) {
  Queue q0 {size, 0, 0, nullptr};
  q0.que = new float* [size];
  for (int i = 0; i < size; ++i) {
    q0.que[i] = nullptr;
  }
  return q0;
}

int enqueue(Queue* const queue, float* x) {
  float* p = queue->que[queue->end];
  if (p) {
    return FAILURE;
  }

  queue->que[queue->end] = x;
  queue->end += 1;
  queue->end = queue->end > queue->size? 0 : queue->end;
  return SUCCESS;
}

float* dequeue(Queue* const queue) {
  float* const p = queue->que[queue->begin];
  if (!p) {
    return FAILURE;
  }

  queue->begin += 1;
  queue->begin = queue->begin > queue->size? 0 : queue->begin;
  return p;
}

int main() {
  float nums[] = {1, 2, 3, 4, 5};
  const size_t N = sizeof(nums) / sizeof(nums[0]);
  Queue Q0 = mk_queue(N);

  for (int i = 0; i < N; ++i) {
    int success = enqueue(&Q0, &nums[i]);
    printf("Queue begin = %d, end = %d <done: %d>\n", Q0.begin, Q0.end, success);
  }
  printf("* enqeue done.\n");
  
  for (int i = 0; i < N; ++i) {
    float* f_ptr = dequeue(&Q0);
    if (f_ptr)
      printf("Queue begin = %d, end = %d => %f\n", Q0.begin, Q0.end, *f_ptr);
    else
      printf("Queue begin = %d, end = %d => FAILED\n", Q0.begin, Q0.end);
  }
  printf("* deqeue done.\n");

  delete [] Q0.que;
  return 0;
}

  
