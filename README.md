# SIMD benchmarks for 64x2 operations

Reusing some [benchmarks][1] from [this paper][0] and elsewhere ported to cpp
and using 64 bit types (uint64 and double), compiled down to Wasm using
Emscripten.

## Results

benchmark       | scalar (ms) | simd (ms)
--------------- | ----------- | -----------
matrix_multiply | 2.529000    | 0.080000
mandelbrot      | 8198.565000 | 5507.096000
double_sum      | 2713.226000 | 1678.963000
int64_average   | 1561.061000 | 859.337000
double_average  | 1580.352000 | 1052.753000

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

## Distribution of instructions

A one-liner to dump 64x2 instructions used:

```
wasm-objdump -d matrix_multiply_intrinsics.wasm | awk '/64x2/ { a = substr($0, index($0, "|") + 1); sub(/ +/, "", a); print a;}' | sort | uniq -c
```

For matrix_multiply:

24 f64x2.add 32 f64x2.mul 8 f64x2.replace_lane 1 8 f64x2.splat

## Distribution of instructions

A one-liner to dump 64x2 instructions used in benchmarks:

```
$ wasm-objdump -d matrix_multiply_intrinsics.wasm mandelbrot-game-simd.wasm double_sum_64.wasm int64_average_64.wasm double_average_64.wasm | awk '/64x2/ { a = substr($0, index($0, "|") + 1); sub(/ +/, "", a); print a;}' | sort | uniq -c | sort -rn
    193 f64x2.mul
    182 f64x2.add
     48 f64x2.sub
     15 f64x2.splat
     12 i64x2.splat
     11 f64x2.replace_lane 1
      9 f64x2.extract_lane 1
      9 f64x2.extract_lane 0
      8 i64x2.replace_lane 1
      4 i64x2.any_true
      4 i64x2.add
      3 i64x2.shr_s
      3 i64x2.shl
      3 i64x2.extract_lane 1
      3 i64x2.extract_lane 0
```

[0]: McCutchan, John, et al. "A SIMD programming model for Dart, JavaScript,
and other dynamically typed scripting languages." Proceedings of the 2014
Workshop on Programming models for SIMD/Vector processing. ACM, 2014.

[1]: https://github.com/tc39/ecmascript_simd/tree/master/src/benchmarks
