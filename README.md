# SIMD benchmarks for 64x2 operations

Reusing some [benchmarks][1] from [this paper][0] and elsewhere ported to cpp
and using 64 bit types (uint64 and double), compiled down to Wasm using
Emscripten.

Also see
[slides](https://docs.google.com/presentation/d/1WNtU2VZuKSAx-RHwsFlZH-rrWiuf4BiAp4GK8CiolkA/edit#slide=id.p)
presented at CG-08-20 summarizing results.

## Results

### x64 (Xeon(R) E5 28 cores)

benchmark       | scalar (ms) | simd (ms)
--------------- | ----------- | -----------
matrix_multiply | 2.529000    | 0.080000
mandelbrot      | 8198.565000 | 5507.096000
double_sum      | 2713.226000 | 1678.963000
int64_average   | 1561.061000 | 859.337000
double_average  | 1580.352000 | 1052.753000

gemm_benchmark_test (excerpt):

```
Comparing ../lapack/gemm_benchmark_test.json to ../lapack/gemm_benchmark_test_unimplemented.json
Benchmark                      Time             CPU      Time Old      Time New       CPU Old       CPU New
-----------------------------------------------------------------------------------------------------------
BM_DGEMM/3                  -0.0360         -0.0360            97            93            97            93
BM_DGEMM/7                  +0.0389         +0.0389           484           503           484           503
BM_DGEMM/31                 -0.2598         -0.2598         17291         12799         17291         12799
BM_DGEMM/100                -0.4010         -0.4010        544508        326154        544509        326154
BM_DGEMM/150                -0.3985         -0.3985       1820423       1095016       1820426       1095017
BM_DGEMM/200                -0.3629         -0.3629       4482777       2855894       4482790       2855898
BM_DGEMM/600                -0.3738         -0.3738     118694833      74328555     118695167      74328889
```

### arm64 (Pixel 3, sdm845)

benchmark       | scalar (ms)  | simd (ms)
--------------- | ------------ | ------------
matrix_multiply | 17.926000    | 0.004000
mandelbrot      | 10891.265000 | 11661.831000
double_sum      | 5204.213000  | 2819.051000
int64_average   | 2633.133000  | 1331.012000
double_average  | 2468.422000  | 1537.558000

gemm_benchmark_test (excerpt):

```
Comparing ../lapack/results/arm64_gemm_benchmark_test.json to ../lapack/results/arm64_gemm_benchmark_test_unimplemented.json
Benchmark                      Time             CPU      Time Old      Time New       CPU Old       CPU New
-----------------------------------------------------------------------------------------------------------
BM_DGEMM/3                  +0.0003         +0.0003           322           322           322           322
BM_DGEMM/7                  -0.0661         -0.0661          1674          1563          1674          1563
BM_DGEMM/31                 -0.2194         -0.2194         62874         49078         62875         49078
BM_DGEMM/100                -0.2694         -0.2694       1901978       1389584       1901984       1389590
BM_DGEMM/150                -0.2729         -0.2729       6489519       4718500       6489546       4718507
BM_DGEMM/200                -0.2764         -0.2764      15284304      11059603      15284457      11059730
BM_DGEMM/600                -0.2762         -0.2762     415962001     301087500     415968500     301094000
```

## Benchmarks

There are two kinds of benchmarks:

1.  cpp source containing normal code that is then compiled with -O3 and
    autovectorized by LLVM, generating simd instructions
2.  cpp source containing two separate implementations, one scalar and the other
    simd (using intrinsics)

The first is useful for looking at what the toolchain can do to autovectorize
and speed up code. The second is useful when the algorithm is not simply
autovectorizable, and shows how using the intrinsics can affect performance.

Examples of working benchmarks and how to run them:

```
$ make matrix_multiply.bench
timing (scalar): 2.529000
timing (simd): 0.080000
```

```
$ make mandelbrot.bench
timing: 8198.565000
timing: 5507.096000
```

```
$ make double_sum.bench
--- double_sum
v128: 0  64x2: 0 32x4:  0
timing: 2713.226000
result: 32766.000000
v128: 1  64x2: 0 32x4:  6
timing: 2696.457000
result: 32766.000000
v128: 6  64x2: 11 32x4:  0
timing: 1678.963000
result: 32766.000000
```

```
$ make int64_average.bench
shared:WARNING: LLVM version appears incorrect (seeing "10.0", expected "9.0")
shared:WARNING: LLVM version appears incorrect (seeing "10.0", expected "9.0")
shared:WARNING: LLVM version appears incorrect (seeing "10.0", expected "9.0")
--- int64_average
v128: 0  64x2: 0 32x4:  0
timing: 1561.061000
result: 8191
v128: 0  64x2: 0 32x4:  0
timing: 1491.316000
result: 8191
v128: 3  64x2: 8 32x4:  0
timing: 859.337000
result: 8191
```

```
$ make double_average.bench
--- double_average
v128: 0  64x2: 0 32x4:  0
timing: 1580.352000
result: 8191.500000
v128: 1  64x2: 0 32x4:  6
timing: 1605.111000
result: 8191.500000
v128: 3  64x2: 14 32x4:  0
timing: 1052.753000
result: 8191.500000
```

There is another benchmark for running
[SIMD-oriented Fast Mersenne Twister(SFMT)](http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/SFMT/index.html).
A Wasm port is available [here](https://github.com/penzn/dSFMT-wasm). To run the
test:

```
$ make emcc-std emcc-simd
$ d8 --experimental-wasm-simd test-simd128-M19937.js -- -s
consumed time for generating 100000000 randoms.
ST BLOCK [0, 1) AVE: 125ms.
ST BLOCK (0, 1] AVE: 120ms.
ST BLOCK (0, 1) AVE: 120ms.
ST BLOCK [1, 2) AVE: 105ms.
ST SEQ [0, 1) 1 AVE: 415ms.
ST SEQ [0, 1) 2 AVE: 309ms.
total = 500014655.815776
ST SEQ (0, 1] 1 AVE: 413ms.
ST SEQ (0, 1] 2 AVE: 317ms.
total = 500035344.184224
ST SEQ (0, 1) 1 AVE: 384ms.
ST SEQ (0, 1) 2 AVE: 317ms.
total = 500014655.815776
ST SEQ [1, 2) 1 AVE: 404ms.
ST SEQ [1, 2) 2 AVE: 304ms.
total = 1500064655.815183

$ d8 test-std-M19937.js -- -s
consumed time for generating 100000000 randoms.
ST BLOCK [0, 1) AVE: 199ms.
ST BLOCK (0, 1] AVE: 197ms.
ST BLOCK (0, 1) AVE: 207ms.
ST BLOCK [1, 2) AVE: 171ms.
ST SEQ [0, 1) 1 AVE: 451ms.
ST SEQ [0, 1) 2 AVE: 355ms.
total = 500014655.815776
ST SEQ (0, 1] 1 AVE: 452ms.
ST SEQ (0, 1] 2 AVE: 362ms.
total = 500035344.184224
ST SEQ (0, 1) 1 AVE: 421ms.
ST SEQ (0, 1) 2 AVE: 354ms.
total = 500014655.815776
ST SEQ [1, 2) 1 AVE: 447ms.
ST SEQ [1, 2) 2 AVE: 354ms.
total = 1500064655.815183
```

## Distribution of instructions

A one-liner to dump 64x2 instructions used in benchmarks:

```
$ wasm-objdump -d test-simd128-M19937.wasm matrix_multiply_intrinsics.wasm mandelbrot-game-simd.wasm double_sum_64.wasm int64_average_64.wasm double_average_64.wasm | awk '/64x2/ { a = substr($0, index($0, "|") + 1); sub(/ +/, "", a); print a;}' | sort | uniq -c | sort -rn
    193 f64x2.mul
    188 f64x2.add
     51 f64x2.sub
     26 i64x2.shl
     24 f64x2.splat
     23 i64x2.shr_u
     17 i64x2.splat
     11 f64x2.replace_lane 1
      9 f64x2.extract_lane 1
      9 f64x2.extract_lane 0
      8 i64x2.replace_lane 1
      6 i64x2.add
      4 i64x2.extract_lane 0
      4 i64x2.any_true
      3 i64x2.shr_s
      3 i64x2.extract_lane 1
```

\[0]: McCutchan, John, et al. "A SIMD programming model for Dart, JavaScript,
and other dynamically typed scripting languages." Proceedings of the 2014
Workshop on Programming models for SIMD/Vector processing. ACM, 2014.

[1]: https://github.com/tc39/ecmascript_simd/tree/master/src/benchmarks
