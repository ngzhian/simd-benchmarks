#include <stdio.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#else
double emscripten_get_now() {
  return 0.0;
}
#endif

#define ITERATIONS 100000
#define SIZE 16384


double bench(double arr[]) {
  double sum = 0.0;

  for (int i = 0; i < SIZE; i++) {
    sum += arr[i];
  }

  return (sum/SIZE);
}

int main() {
  double result;
  double arr[SIZE];

  for (int i = 0; i < SIZE; i++) {
    arr[i] = (double) i;
  }

  double start = emscripten_get_now();
  for (int i = 0; i < ITERATIONS; i++) {
    result = bench(arr);
  }

  printf("timing: %f\n", emscripten_get_now() - start);
  printf("result: %f\n", result);
}
