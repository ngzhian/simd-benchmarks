# SIMD benchmarks for 64x2 operations

Reusing some benchmarks from the paper "A SIMD programming model for dart,
javascript,and other dynamically typed
scripting languages" but ported to cpp and using 64 bit types (uint64 and
double). Source code for those benchmarks are at [0]

## Benchmarks

There are two kinds of benchmarks:

1. cpp source containing normal code that is then compiled with -O3 and
autovectorized by LLVM, generating simd instructions
2. cpp source containing two separate implementations, one scalar and the other
simd (using intrinsics)

The first is useful for looking at what the toolchain can do to autovectorize
and speed up code. The second is useful when the algorithm is not simply
autovectorizable, and shows how using the intrinsics can affect performance.

Examples of working benchmarks and how to run them:

```
$ make matrix_multply.bench
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

double_average.cpp is a benchmark that shows that 64x2 instructions has no
effect. The autovectorizer cannot vetorize the addition of floats due to nan
propagations.

[0] https://github.com/tc39/ecmascript_simd/tree/master/src/benchmarks
