#include <stdio.h>
#include <cstdint>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#else
double emscripten_get_now() {
  return 0.0;
}
#endif

#define ITERATIONS 100000
#define SIZE 16384


int64_t bench(int64_t arr[]) {
  int64_t sum = 0;

  for (int i = 0; i < SIZE; i++) {
    sum += arr[i];
  }

  return (sum/SIZE);
}

int main() {
  int64_t result;
  int64_t arr[SIZE];

  for (int i = 0; i < SIZE; i++) {
    arr[i] = (int64_t) i;
  }

  int64_t start = emscripten_get_now();
  for (int i = 0; i < ITERATIONS; i++) {
    result = bench(arr);
  }

  printf("timing: %f\n", emscripten_get_now() - start);
  printf("result: %lld\n", result);
}
