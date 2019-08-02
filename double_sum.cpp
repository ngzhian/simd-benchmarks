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


void bench(double xs[], double ys[], double zs[]) {
  for (int i = 0; i < SIZE; i++) {
    zs[i] = xs[i] + ys[i];
  }
}

int main() {
  double xs[SIZE];
  double ys[SIZE];
  double zs[SIZE];

  for (int i = 0; i < SIZE; i++) {
    xs[i] = (double) i;
    ys[i] = (double) i;
    zs[i] = 0.0f;
  }

  double start = emscripten_get_now();

  for (int i = 0; i < ITERATIONS; i++) {
    bench(xs, ys, zs);
  }

  printf("timing: %f\n", emscripten_get_now() - start);
  printf("result: %f\n", zs[SIZE-1]);
}
