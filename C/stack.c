// simple stack implementation
#include <stdlib.h>
#include <stdio.h>

enum Flag {Failure, Success};

struct Stack {
  const size_t size;
  size_t head;
  float** array;
};

struct Stack mk_stack(const size_t size) {
  float** stackmem_ptr = (float**) malloc(size * sizeof(float*));
  struct Stack st = {size, 0, stackmem_ptr};
  return st;
}

enum Flag push(struct Stack* stack, float* x_ptr) {
  if (stack->head < stack->size) {
    stack->head = stack->head;
    stack->array[stack->head] = x_ptr;
    printf("pushed %f to <%d>.\n", *(stack->array[stack->head]), stack->head);
    stack->head = stack->head + 1;
    return Success;
  }
  return Failure;
}

float* pop(struct Stack* stack) {
  if (stack->head > 0) {
    stack->head = stack->head - 1;
    printf("read at %d: ", stack->head);
    float* ptr = stack->array[stack->head];
    if (ptr != NULL) {
      printf("popped %f .\n", *ptr);
    }
    return ptr;
    
  }
  return NULL;
}

int main() {
  const size_t N = 5;
  float nums[5] = {1, 2, 3, 4, 5};
  struct Stack stack = mk_stack(N);
  if (stack.array == NULL) {
    printf("Allocation failed.");
    return -1;
  }
  printf("stack at %d\n", stack.head);
  enum Flag success = Failure;
  for (int i = 0; i < N+3; i = i + 1) {
    success = push(&stack, &nums[i]);
    printf("%d) stack pushed: success = %d\n", i, success);
    printf("%d) stack at %d\n", i, stack.head);
  }

  printf("done pushing.\n");

  for (int i = 0; i < N+3; i = i + 1) {
    float* x = pop(&stack);
    if (x != NULL) {
      printf("stack poped %f\n", *x);
      printf("stack at %d\n", stack.head);
    }
    else
      printf("stack pop failed\n");
  }
  printf("done popping.\n");
  free(stack.array);
  return 0;
}
