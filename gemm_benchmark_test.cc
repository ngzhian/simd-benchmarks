#include "benchmark/benchmark.h"
#include <Eigen/Dense>

void BM_DGEMM(benchmark::State& state) {
  // First resume to ensure the timer is running.  Without this benchmark
  // gives garbage results.
  // You do not need to do this with the opensource version of benchmark.
  state.ResumeTiming();
  state.PauseTiming();
  const int size = state.range(0);
  Eigen::MatrixXd a = Eigen::MatrixXd::Random(size, size);
  Eigen::MatrixXd b = Eigen::MatrixXd::Random(size, size);
  Eigen::MatrixXd c = Eigen::MatrixXd::Random(size, size);
  state.ResumeTiming();
  for (auto _ : state) {
    a.noalias() += b * c;
  }
}

BENCHMARK(BM_DGEMM)
    ->Arg(3)
    ->Arg(4)
    ->Arg(5)
    ->Arg(6)
    ->Arg(7)
    ->Arg(8)
    ->Arg(9)
    ->Arg(10)
    ->Arg(15)
    ->Arg(16)
    ->Arg(20)
    ->Arg(24)
    ->Arg(28)
    ->Arg(31)
    ->Arg(32)
    ->Arg(40)
    ->Arg(63)
    ->Arg(64)
    ->Arg(80)
    ->Arg(100)
    ->Arg(128)
    ->Arg(150)
    ->Arg(200)
    ->Arg(256)
    ->Arg(300)
    ->Arg(400)
    ->Arg(500)
    ->Arg(600);
// any larger and it OOMs

BENCHMARK_MAIN();
