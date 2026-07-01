#include <benchmark/benchmark.h>
#include "matching_engine.h"

static void BM_OrderLifecycle(benchmark::State& state){
    lob::LimitOrderBook engine;
    uint64_t id = 1;

    for (auto _ : state){
        engine.add_order(id, true, 10050, 100);
        engine.cancel_order(id);

        id++;

        if (id > 999999) {
            id = 1;
        }
    }
}

BENCHMARK(BM_OrderLifecycle);

BENCHMARK_MAIN();