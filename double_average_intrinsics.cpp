#include <stdio.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <wasm_simd128.h>
#else
double emscripten_get_now() {
  return 0.0;
}
#endif

#define ITERATIONS 100000
#define SIZE 16384


double bench(double arr[]) {
  double sum = 0.0;
  __f64x2 s = wasm_f64x2_splat(0.0f);

  for (int i = 0; i < SIZE; i+=2) {
    s = wasm_f64x2_add(s, wasm_v128_load(arr+i));
  }

  return (s[0] + s[1]) / SIZE;
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
