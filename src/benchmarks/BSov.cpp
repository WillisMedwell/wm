#include <benchmark/benchmark.h>

#include "Sov.hpp"

#include <random>
#include <vector>

namespace TSov {
struct Data {
    std::string a;
    uint64_t b;
    bool c;
    Data(std::string a_, uint64_t b_, bool c_) : a(a_), b(b_), c(c_) { }
};
}

static void BM_SovAllFieldsIteration(benchmark::State& state)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> uint64_dist(0, 100000);
    std::uniform_int_distribution<int> bool_dist(0, 1);
    int N = state.range(0);
    Sov<std::string, uint64_t, bool> sov;
    for (int i = 0; i < N; i++) {
        sov.pushBack(std::to_string(i), uint64_dist(gen), static_cast<bool>(bool_dist(gen)));
    }
    size_t sum = 0;
    for (auto _ : state) {

        for (auto [a, b, c] : sov) {
            sum += a.size() + b + c;
        }
        benchmark::DoNotOptimize(sov);
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_SovAllFieldsIteration)
    ->RangeMultiplier(2)
    ->Range(1 << 6, 1 << 17);

static void BM_VecAllFieldsIteration(benchmark::State& state)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> uint64_dist(0, 100000);
    std::uniform_int_distribution<int> bool_dist(0, 1);
    int N = state.range(0);
    std::vector<TSov::Data> vec;
    for (int i = 0; i < N; i++) {
        vec.emplace_back(std::to_string(i), uint64_dist(gen), static_cast<bool>(bool_dist(gen)));
    }
    size_t sum = 0;
    for (auto _ : state) {

        for (auto& data : vec) {
            sum += data.a.size() + data.b + data.c;
        }
        benchmark::DoNotOptimize(vec);
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_VecAllFieldsIteration)
    ->RangeMultiplier(2)
    ->Range(1 << 6, 1 << 17);

static void BM_Sov1FieldIteration(benchmark::State& state)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> uint64_dist(0, 100000);
    std::uniform_int_distribution<int> bool_dist(0, 1);
    int N = state.range(0);
    Sov<std::string, uint64_t, bool> sov;
    for (int i = 0; i < N; i++) {
        sov.pushBack(std::to_string(i), uint64_dist(gen), static_cast<bool>(bool_dist(gen)));
    }
    size_t sum = 0;
    for (auto _ : state) {

        for (auto& a : sov.field<std::string>()) {
            sum += a.size();
        }
        for (auto& b : sov.field<uint64_t>()) {
            sum += b;
        }
        for (auto& c : sov.field<bool>()) {
            sum += c;
        }
        benchmark::DoNotOptimize(sov);
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_Sov1FieldIteration)
    ->RangeMultiplier(2)
    ->Range(1 << 6, 1 << 17);

static void BM_Vec1FieldIteration(benchmark::State& state)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> uint64_dist(0, 100000);
    std::uniform_int_distribution<int> bool_dist(0, 1);
    int N = state.range(0);
    std::vector<TSov::Data> vec;
    for (int i = 0; i < N; i++) {
        vec.emplace_back(std::to_string(i), uint64_dist(gen), static_cast<bool>(bool_dist(gen)));
    }
    size_t sum = 0;
    for (auto _ : state) {

        for (auto& data : vec) {
            sum += data.a.size();
        }
        for (auto& data : vec) {
            sum += data.b;
        }
        for (auto& data : vec) {
            sum += data.c;
        }
        benchmark::DoNotOptimize(vec);
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_Vec1FieldIteration)
    ->RangeMultiplier(2)
    ->Range(1 << 6, 1 << 17);