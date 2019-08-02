/* Benchmarks ported from tc38/ecmascript_simd.
 * See
 * https://github.com/tc39/ecmascript_simd/blob/master/src/benchmarks/matrix-multiplication.js
 */
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

// 4x4 square matrix
#define MATRIX_SIZE 16

void bench_simd(double T1x[], double T2x[], double Outx[]) {
  v128_t a0 = wasm_v128_load(T1x+ 0);
  v128_t a1 = wasm_v128_load(T1x+ 2);
  v128_t a2 = wasm_v128_load(T1x+ 4);
  v128_t a3 = wasm_v128_load(T1x+ 6);
  v128_t a4 = wasm_v128_load(T1x+ 8);
  v128_t a5 = wasm_v128_load(T1x+ 10);
  v128_t a6 = wasm_v128_load(T1x+ 12);
  v128_t a7 = wasm_v128_load(T1x+ 14);

  v128_t b0 = wasm_v128_load(T2x+ 0);
  v128_t b1 = wasm_v128_load(T2x+ 2);

  wasm_v128_store(Outx+0,
                  wasm_f64x2_add(
                      wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b0, 0)), a0),
                      wasm_f64x2_add(
                          wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b0, 1)), a2),
                          wasm_f64x2_add(
                              wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b1, 0)), a4),
                              wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b1, 1)), a6)))));
  wasm_v128_store(Outx+2,
                  wasm_f64x2_add(
                      wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b0, 0)), a1),
                      wasm_f64x2_add(
                          wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b0, 1)), a3),
                          wasm_f64x2_add(
                              wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b1, 0)), a5),
                              wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b1, 1)), a7)))));

  v128_t b2 = wasm_v128_load(T2x+ 4);
  v128_t b3 = wasm_v128_load(T2x+ 6);

  wasm_v128_store(Outx+4,
                  wasm_f64x2_add(
                      wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b2, 0)), a0),
                      wasm_f64x2_add(
                          wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b2, 1)), a2),
                          wasm_f64x2_add(
                              wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b3, 0)), a4),
                              wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b3, 1)), a6)))));
  wasm_v128_store(Outx+6,
                  wasm_f64x2_add(
                      wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b2, 0)), a1),
                      wasm_f64x2_add(
                          wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b2, 1)), a3),
                          wasm_f64x2_add(
                              wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b3, 0)), a5),
                              wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b3, 1)), a7)))));

  v128_t b4 = wasm_v128_load(T2x+ 8);
  v128_t b5 = wasm_v128_load(T2x+ 10);

  wasm_v128_store(Outx+8,
                  wasm_f64x2_add(
                      wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b4, 0)), a0),
                      wasm_f64x2_add(
                          wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b4, 1)), a2),
                          wasm_f64x2_add(
                              wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b5, 0)), a4),
                              wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b5, 1)), a6)))));
  wasm_v128_store(Outx+10,
                  wasm_f64x2_add(
                      wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b4, 0)), a1),
                      wasm_f64x2_add(
                          wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b4, 1)), a3),
                          wasm_f64x2_add(
                              wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b5, 0)), a5),
                              wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b5, 1)), a7)))));

  v128_t b6 = wasm_v128_load(T2x+ 12);
  v128_t b7 = wasm_v128_load(T2x+ 14);

  wasm_v128_store(Outx+12,
                  wasm_f64x2_add(
                      wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b6, 0)), a0),
                      wasm_f64x2_add(
                          wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b6, 1)), a2),
                          wasm_f64x2_add(
                              wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b7, 0)), a4),
                              wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b7, 1)), a6)))));
  wasm_v128_store(Outx+14,
                  wasm_f64x2_add(
                      wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b6, 0)), a1),
                      wasm_f64x2_add(
                          wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b6, 1)), a3),
                          wasm_f64x2_add(
                              wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b7, 0)), a5),
                              wasm_f64x2_mul(wasm_f64x2_splat(wasm_f64x2_extract_lane(b7, 1)), a7)))));
}

void bench_scalar(double x[], double y[], double z[]) {
      double a00 = x[0];
      double a01 = x[1];
      double a02 = x[2];
      double a03 = x[3];
      double a10 = x[4];
      double a11 = x[5];
      double a12 = x[6];
      double a13 = x[7];
      double a20 = x[8];
      double a21 = x[9];
      double a22 = x[10];
      double a23 = x[11];
      double a30 = x[12];
      double a31 = x[13];
      double a32 = x[14];
      double a33 = x[15];

      double b0 = y[0];
      double b1 = y[1];
      double b2 = y[2];
      double b3 = y[3];
      z[0] = b0*a00 + b1*a10 + b2*a20 + b3*a30;
      z[1] = b0*a01 + b1*a11 + b2*a21 + b3*a31;
      z[2] = b0*a02 + b1*a12 + b2*a22 + b3*a32;
      z[3] = b0*a03 + b1*a13 + b2*a23 + b3*a33;

      b0 = y[4];
      b1 = y[5];
      b2 = y[6];
      b3 = y[7];
      z[4] = b0*a00 + b1*a10 + b2*a20 + b3*a30;
      z[5] = b0*a01 + b1*a11 + b2*a21 + b3*a31;
      z[6] = b0*a02 + b1*a12 + b2*a22 + b3*a32;
      z[7] = b0*a03 + b1*a13 + b2*a23 + b3*a33;

      b0 = y[8];
      b1 = y[9];
      b2 = y[10];
      b3 = y[11];
      z[8] = b0*a00 + b1*a10 + b2*a20 + b3*a30;
      z[9] = b0*a01 + b1*a11 + b2*a21 + b3*a31;
      z[10] = b0*a02 + b1*a12 + b2*a22 + b3*a32;
      z[11] = b0*a03 + b1*a13 + b2*a23 + b3*a33;

      b0 = y[12];
      b1 = y[13];
      b2 = y[14];
      b3 = y[15];
      z[12] = b0*a00 + b1*a10 + b2*a20 + b3*a30;
      z[13] = b0*a01 + b1*a11 + b2*a21 + b3*a31;
      z[14] = b0*a02 + b1*a12 + b2*a22 + b3*a32;
      z[15] = b0*a03 + b1*a13 + b2*a23 + b3*a33;
}

void sanity_check(double z1[], double z2[]) {
  for (int i = 0; i < MATRIX_SIZE; i++) {
    if (z1[i] != z2[i]) {
      printf("mismatch at %d: %f v.s. %f\n", i, z1[i], z2[i]);
    }
  }
}

int main() {
  /* double result; */
  double start;

  double x[MATRIX_SIZE];
  double y[MATRIX_SIZE];
  double z_scalar[MATRIX_SIZE];
  double z_simd[MATRIX_SIZE];

  for (int i = 0; i < MATRIX_SIZE; i++) {
    x[i] = (double) i;
    y[i] = (double) i;
    z_scalar[i] = 0.0f;
    z_simd[i] = 0.0f;
  }

  bench_scalar(x, y, z_scalar);
  bench_simd(x, y, z_simd);
  sanity_check(z_scalar, z_simd);

  start = emscripten_get_now();
  for (int i = 0; i < ITERATIONS; i++) {
    bench_scalar(x, y, z_scalar);
  }
  printf("timing (scalar): %f\n", emscripten_get_now() - start);

  start = emscripten_get_now();
  for (int i = 0; i < ITERATIONS; i++) {
    bench_simd(x, y, z_simd);
  }
  printf("timing (simd): %f\n", emscripten_get_now() - start);
}
