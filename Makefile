# options when source code uses benchmark
BENCHMARK_DIR=../benchmark
BENCHMARK_INCLUDE=$(BENCHMARK_DIR)/include
BENCHMARK_LIB=$(BENCHMARK_DIR)/build/src/libbenchmark.a
BENCHMARK_OPTS=-I$(BENCHMARK_INCLUDE) $(BENCHMARK_LIB)

EIGEN_DIR=../eigen-git-mirror
EIGEN_INCLUDE=$(EIGEN_DIR)
EIGEN_OPTS=-I$(EIGEN_INCLUDE)

EMCC ?= emcc
EMCC_OPTS=-O3
D8 ?= d8
D8_FLAGS=--experimental-wasm-simd

.PHONY: all
all: bench

.PRECIOUS: %.js %_32.js %_64.js %_intrinsics.js

SIMD_OPTS=-msimd128
SIMD_64_OPTS=$(SIMD_OPTS) -munimplemented-simd128

# APPS=double_average

%.js: %.cpp
	@$(EMCC) $(EMCC_OPTS) $< -o $@

%_32.js: %.cpp
	@$(EMCC) $(EMCC_OPTS) $< -o $@ $(SIMD_OPTS)

%_64.js: %.cpp
	@$(EMCC) $(EMCC_OPTS) $< -o $@ $(SIMD_64_OPTS)

%_intrinsics.js: %_intrinsics.cpp
	@$(EMCC) $(EMCC_OPTS) $< -o $@ $(SIMD_64_OPTS)

%.ibench: %_intrinsics.js
	@echo --- $*
	@./wasm-simd-dis.sh $*_intrinsics.wasm
	@$(D8) $(D8_FLAGS) $*_intrinsics.js

%.bench: %.js %_32.js %_64.js
	@echo --- $*
	@./wasm-simd-dis.sh $*.wasm
	@$(D8) $(D8_FLAGS) $*.js
	@./wasm-simd-dis.sh $*_32.wasm
	@$(D8) $(D8_FLAGS) $*_32.js
	@./wasm-simd-dis.sh $*_64.wasm
	@$(D8) $(D8_FLAGS) $*_64.js
	@echo

.PHONY: matrix_multiply.bench
matrix_multiply.bench: matrix_multiply.ibench

%.wat: %.wasm
	@wasm2wat --enable-simd $< -o $@

%.wats: %.wat %_32.wat $.64.wat

.PHONY: bench
bench: matrix_multiply.bench mandelbrot.bench double_sum.bench int64_average.bench double_average.bench

.PHONY: clean
clean:
	rm -f *.js *.wasm *.wat

# -s EXIT_RUNTIME=1 -s ASSERTIONS=1
# mandelbrot from benchmarks game needs special compilation flags
MANDELBROT_GAME_OPTS=-O3 -std=c++11 -s TOTAL_MEMORY=268435456
SCALAR_OPTS=-DMANDELBROT_SCALAR=1
MANDELBROT_SRC=mandelbrot-game.cpp
MANDELBROT_SCALAR_OUT=mandelbrot-game-scalar.js
MANDELBROT_SIMD_OUT=mandelbrot-game-simd.js
# width and height, useful for benchmarks that run slow
MANDELBROT_W_H ?= 16000

.PHONY: mandelbrot-game
mandelbrot-game: $(MANDELBROT_SRC)
	@$(EMCC) $(MANDELBROT_GAME_OPTS) $(SCALAR_OPTS) $(MANDELBROT_SRC) -o $(MANDELBROT_SCALAR_OUT)
	@$(EMCC) $(MANDELBROT_GAME_OPTS) $(SIMD_64_OPTS) $(MANDELBROT_SRC) -o $(MANDELBROT_SIMD_OUT)

.PHONY: mandelbrot.bench
mandelbrot.bench: run-mandelbrot-game
# Run the mandelbrot set benchmark to get the timings
.PHONY: run-mandelbrot-game
run-mandelbrot-game: mandelbrot-game
	@echo --- mandelbrot
	@./wasm-simd-dis.sh $(MANDELBROT_SCALAR_OUT:.js=.wasm)
	@./wasm-simd-dis.sh $(MANDELBROT_SIMD_OUT:.js=.wasm)
	@$(D8) $(D8_FLAGS) $(MANDELBROT_SCALAR_OUT) -- $(MANDELBROT_W_H)
	@$(D8) $(D8_FLAGS) $(MANDELBROT_SIMD_OUT) -- $(MANDELBROT_W_H)

.PHONY: mandelbrot-game-gen-image
mandelbrot-game-gen-image: $(MANDELBROT_SRC)
	$(EMCC) $(MANDELBROT_GAME_OPTS) $(SCALAR_OPTS) $(MANDELBROT_SRC) -o $(MANDELBROT_SCALAR_OUT) -DGEN_IMAGE=1
	$(EMCC) $(MANDELBROT_GAME_OPTS) $(SIMD_64_OPTS) $(MANDELBROT_SRC) -o $(MANDELBROT_SIMD_OUT) -DGEN_IMAGE=1

# Run the mandelbrot set benchmark and generate pbm images
.PHONY: run-mandelbrot-game-gen-image
run-mandelbrot-game-gen-image: mandelbrot-game-gen-image
	@echo --- mandelbrot
	@./wasm-simd-dis.sh $(MANDELBROT_SCALAR_OUT:.js=.wasm)
	@./wasm-simd-dis.sh $(MANDELBROT_SIMD_OUT:.js=.wasm)
	@time $(D8) $(D8_FLAGS) $(MANDELBROT_SCALAR_OUT) > scalar.pbm.tmp
	@python3 postprocess.py scalar.pbm.tmp > scalar.pbm
	@time $(D8) $(D8_FLAGS) $(MANDELBROT_SIMD_OUT) > simd.pbm.tmp
	@python3 postprocess.py simd.pbm.tmp > simd.pbm
	@diff -q scalar.pbm simd.pbm
	@rm scalar.pbm.tmp simd.pbm.tmp

# Mandelbrot set from dart repository copied from https://github.com/tc39/ecmascript_simd/blob/master/src/benchmarks/mandelbrot.js
.PHONY: mandelbrot-dart
mandelbrot-dart: mandel-dart.cpp
	$(EMCC) $(MANDELBROT_GAME_OPTS) $(SCALAR_OPTS) mandel-dart.cpp -o mandel-dart-scalar.js
	@time $(D8) $(D8_FLAGS) mandel-dart-scalar.js
	$(EMCC) $(MANDELBROT_GAME_OPTS) $(SIMD_64_OPTS) mandel-dart.cpp -o mandel-dart.js
	@time $(D8) $(D8_FLAGS) mandel-dart.js

# Cross compilation instructions at https://v8.dev/docs/cross-compile-arm
# In short:
#   gm android_arm64.release
#   adb push out/android_arm64.release/{icudtl.dat,natives_blob.bin,snapshot_blob.bin,d8} /data/local/tmp/v8/bin
ANDROID_D8_DIR=/data/local/tmp/v8/bin
.PHONY: run-android-arm64
run-android-arm64:
	@adb push matrix_multiply_intrinsics.{js,wasm} $(ANDROID_D8_DIR)
	@adb push $(MANDELBROT_SCALAR_OUT) $(ANDROID_D8_DIR)
	@adb push $(MANDELBROT_SCALAR_OUT:.js=.wasm) $(ANDROID_D8_DIR)
	@adb push $(MANDELBROT_SIMD_OUT) $(ANDROID_D8_DIR)
	@adb push $(MANDELBROT_SIMD_OUT:.js=.wasm) $(ANDROID_D8_DIR)
	@adb push double_sum.{js,wasm} $(ANDROID_D8_DIR)
	@adb push double_sum_64.{js,wasm} $(ANDROID_D8_DIR)
	@adb push int64_average.{js,wasm} $(ANDROID_D8_DIR)
	@adb push int64_average_64.{js,wasm} $(ANDROID_D8_DIR)
	@adb push double_average.{js,wasm} $(ANDROID_D8_DIR)
	@adb push double_average_64.{js,wasm} $(ANDROID_D8_DIR)
	adb shell 'cd $(ANDROID_D8_DIR) && ./d8 $(D8_FLAGS) $(ANDROID_D8_DIR)/matrix_multiply_intrinsics.js'
	adb shell 'cd $(ANDROID_D8_DIR) && ./d8 $(D8_FLAGS) $(ANDROID_D8_DIR)/$(MANDELBROT_SCALAR_OUT) -- $(MANDELBROT_W_H)'
	adb shell 'cd $(ANDROID_D8_DIR) && ./d8 $(D8_FLAGS) $(ANDROID_D8_DIR)/$(MANDELBROT_SIMD_OUT) -- $(MANDELBROT_W_H)'
	adb shell 'cd $(ANDROID_D8_DIR) && ./d8 $(D8_FLAGS) $(ANDROID_D8_DIR)/double_sum.js'
	adb shell 'cd $(ANDROID_D8_DIR) && ./d8 $(D8_FLAGS) $(ANDROID_D8_DIR)/double_sum_64.js'
	adb shell 'cd $(ANDROID_D8_DIR) && ./d8 $(D8_FLAGS) $(ANDROID_D8_DIR)/int64_average.js'
	adb shell 'cd $(ANDROID_D8_DIR) && ./d8 $(D8_FLAGS) $(ANDROID_D8_DIR)/int64_average_64.js'
	adb shell 'cd $(ANDROID_D8_DIR) && ./d8 $(D8_FLAGS) $(ANDROID_D8_DIR)/double_average.js'
	adb shell 'cd $(ANDROID_D8_DIR) && ./d8 $(D8_FLAGS) $(ANDROID_D8_DIR)/double_average_64.js'

.PHONY: gemm_benchmark_test
gemm_benchmark_test: gemm_benchmark_test.cc
	@$(EMCC) $(EMCC_OPTS) $(EIGEN_OPTS) $(BENCHMARK_OPTS) -std=c++17 $< -o $@.js
	@$(EMCC) $(EMCC_OPTS) $(EIGEN_OPTS) $(BENCHMARK_OPTS) $(SIMD_64_OPTS) -std=c++17 $< -o $@_unimplemented.js
	@echo --- $@
	@./wasm-simd-dis.sh $@.wasm
	@$(D8) $@.js
	@./wasm-simd-dis.sh $@_unimplemented.wasm
	@$(D8) $(D8_FLAGS) $@_unimplemented.js
	@echo
