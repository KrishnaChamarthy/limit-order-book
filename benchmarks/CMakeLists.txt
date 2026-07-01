find_package(benchmark REQUIRED)

add_executable(run_benchmarks bench_engine.cpp)

target_link_libraries(run_benchmarks PRIVATE core_engine benchmark::benchmark)