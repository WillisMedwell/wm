#include <benchmark/benchmark.h>

#include "wm/Sov.hpp"

#include <array>
#include <random>
#include <vector>

using namespace wm;

namespace TSov {
struct Vec2 {
    float x, y;
};
struct Name256 {
    std::array<char, 256> data;
};

struct Name1024 {
    std::array<char, 1024> data;
};
struct Kinematic {
    Vec2 vel;
    Vec2 pos;
};

struct Data {
    uint64_t id;
    Name256 name;
    Kinematic kinematic;
};
struct Data2 {
    uint64_t id;
    Name1024 name;
    Kinematic kinematic;
};
}

static void BM_AllFieldsIteration_Sov(benchmark::State& state)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> uint64_dist(0, 100000);
    std::uniform_int_distribution<int> uint8_dist(0, 255);
    std::uniform_real_distribution<float> float_dist(-20.0f, 20.0f);

    int N = state.range(0);
    Sov<uint64_t, TSov::Name256, TSov::Kinematic> sov;
    for (int i = 0; i < N; i++) {
        sov.pushBack(
            uint64_dist(gen), 
            {}, 
            {float_dist(gen), float_dist(gen), float_dist(gen), float_dist(gen)}
        );
    }
    size_t sum = 0;
    for (auto _ : state) {

        for (auto [a, b, c] : sov) {
            sum += static_cast<size_t>(a + b.data[uint8_dist(gen)] + c.pos.x + c.vel.y);
        }
        benchmark::DoNotOptimize(sov);
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_AllFieldsIteration_Sov)
    ->Name("Use All 280 bytes:Sov")
    ->RangeMultiplier(2)
    ->Range(32, 1 << 17);

static void BM_AllFieldsIteration_Vec(benchmark::State& state)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> uint64_dist(0, 100000);
    std::uniform_int_distribution<int> uint8_dist(0, 255);
    std::uniform_real_distribution<float> float_dist(-20.0f, 20.0f);
    
    int N = state.range(0);
    std::vector<TSov::Data> vec;
    for (int i = 0; i < N; i++) {
        vec.emplace_back(TSov::Data{
            .id = uint64_dist(gen), 
            .name = {}, 
            .kinematic = {float_dist(gen), float_dist(gen), float_dist(gen), float_dist(gen)}}
        );
    }
    size_t sum = 0;
    for (auto _ : state) {

        for (auto& data : vec) {
            sum += static_cast<size_t>(data.id + data.name.data[uint8_dist(gen)] + data.kinematic.pos.x + data.kinematic.vel.y);
        }
        benchmark::DoNotOptimize(vec);
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_AllFieldsIteration_Vec)
    ->Name("Use All 280 bytes:Vec")
    ->RangeMultiplier(2)
    ->Range(32, 1 << 17);

static void BM_1FieldIteration_Sov(benchmark::State& state)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> uint64_dist(0, 100000);
    std::uniform_int_distribution<int> uint8_dist(0, 255);
    std::uniform_real_distribution<float> float_dist(-20.0f, 20.0f);

    int N = state.range(0);
    Sov<uint64_t, TSov::Name256, TSov::Kinematic> sov;
    for (int i = 0; i < N; i++) {
        sov.pushBack(
            uint64_dist(gen), 
            {}, 
            {float_dist(gen), float_dist(gen), float_dist(gen), float_dist(gen)}
        );
    }

    size_t sum = 0;
    for (auto _ : state) {
        for (auto& a : sov.field<TSov::Kinematic>()) {
            sum += static_cast<size_t>(a.pos.x * a.vel.x + a.pos.y * a.vel.y);
        }
        benchmark::DoNotOptimize(sov);
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_1FieldIteration_Sov)
    ->Name("Use 16 bytes of 280 bytes:Sov")
    ->RangeMultiplier(2)
    ->Range(32, 1 << 17);

static void BM_1FieldIteration_Vec(benchmark::State& state)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> uint64_dist(0, 100000);
    std::uniform_int_distribution<int> uint8_dist(0, 255);
    std::uniform_real_distribution<float> float_dist(-20.0f, 20.0f);
    
    int N = state.range(0);
    std::vector<TSov::Data> vec;
    for (int i = 0; i < N; i++) {
        vec.emplace_back(TSov::Data{
            .id = uint64_dist(gen), 
            .name = {}, 
            .kinematic = {float_dist(gen), float_dist(gen), float_dist(gen), float_dist(gen)}}
        );
    }
    size_t sum = 0;
    for (auto _ : state) {

        for (auto& data : vec) {
            sum += static_cast<size_t>(data.kinematic.pos.x * data.kinematic.vel.x + data.kinematic.pos.y * data.kinematic.vel.y);
        }
        benchmark::DoNotOptimize(vec);
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_1FieldIteration_Vec)
    ->Name("Use 16 bytes of 280 bytes:Vec")
    ->RangeMultiplier(2)
    ->Range(32, 1 << 17);

static void BM_1FieldIteration2_Vec(benchmark::State& state)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> uint64_dist(0, 100000);
    std::uniform_int_distribution<int> uint8_dist(0, 255);
    std::uniform_real_distribution<float> float_dist(-20.0f, 20.0f);
    
    int N = state.range(0);
    std::vector<TSov::Kinematic> vec;
    for (int i = 0; i < N; i++) {
        vec.push_back({
            float_dist(gen), float_dist(gen), float_dist(gen), float_dist(gen)}
        );
    }
    size_t sum = 0;
    for (auto _ : state) {

        for (auto& data : vec) {
            sum += static_cast<size_t>(data.pos.x * data.vel.x + data.pos.y * data.vel.y);
        }
        benchmark::DoNotOptimize(vec);
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_1FieldIteration2_Vec)
    ->Name("Use 16 bytes of 280 bytes:Lowest Possible")
    ->RangeMultiplier(2)
    ->Range(32, 1 << 17);

static void BM_1FieldIteration1024_Sov(benchmark::State& state)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> uint64_dist(0, 100000);
    std::uniform_int_distribution<int> uint8_dist(0, 255);
    std::uniform_real_distribution<float> float_dist(-20.0f, 20.0f);

    int N = state.range(0);
    Sov<uint64_t, TSov::Name1024, TSov::Kinematic> sov;
    for (int i = 0; i < N; i++) {
        sov.pushBack(
            uint64_dist(gen), 
            {}, 
            {float_dist(gen), float_dist(gen), float_dist(gen), float_dist(gen)}
        );
    }

    size_t sum = 0;
    for (auto _ : state) {
        for (auto& a : sov.field<TSov::Kinematic>()) {
            sum += static_cast<size_t>(a.pos.x * a.vel.x + a.pos.y * a.vel.y);
        }
        benchmark::DoNotOptimize(sov);
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_1FieldIteration1024_Sov)
    ->Name("Use 16 bytes of 1048 bytes:Sov")
    ->RangeMultiplier(2)
    ->Range(32, 1 << 17);

static void BM_1FieldIteration1024_Vec(benchmark::State& state)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> uint64_dist(0, 100000);
    std::uniform_int_distribution<int> uint8_dist(0, 255);
    std::uniform_real_distribution<float> float_dist(-20.0f, 20.0f);
    
    int N = state.range(0);
    std::vector<TSov::Data2> vec;
    for (int i = 0; i < N; i++) {
        vec.emplace_back(TSov::Data2{
            .id = uint64_dist(gen), 
            .name = {}, 
            .kinematic = {float_dist(gen), float_dist(gen), float_dist(gen), float_dist(gen)}}
        );
    }
    size_t sum = 0;
    for (auto _ : state) {

        for (auto& data : vec) {
            sum += static_cast<size_t>(data.kinematic.pos.x * data.kinematic.vel.x + data.kinematic.pos.y * data.kinematic.vel.y);
        }
        benchmark::DoNotOptimize(vec);
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_1FieldIteration1024_Vec)
    ->Name("Use 16 bytes of 1048 bytes:Vec")
    ->RangeMultiplier(2)
    ->Range(32, 1 << 17);