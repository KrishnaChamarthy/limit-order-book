# Ultra-Low Latency Limit Order Book (LOB)

![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-%23008FBA.svg?style=for-the-badge&logo=cmake&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)

A high-performance, $O(1)$ limit order book and matching engine written in C++20. Designed specifically for simulated high-frequency trading (HFT) workloads, this engine bypasses standard library containers to eliminate dynamic memory allocation on the critical path, achieving nanosecond-level latency.

## ⚡ Performance & Benchmarks

The engine's throughput and latency were profiled using **Google Benchmark**. The benchmark simulates a full order lifecycle: aggressive allocation, insertion, hash map lookup, unlinking, and deallocation.

- **Latency:** `23.7 ns` per order lifecycle.
- **Throughput:** `~42,000,000` operations per second (single-threaded).

```text
------------------------------------------------------------
Benchmark                  Time             CPU   Iterations
------------------------------------------------------------
BM_OrderLifecycle       23.7 ns         23.7 ns     29305133

```

_(Hardware: Apple M-Series ARM64 / Note: Production deployments targeting x86-64 Linux with CPU pinning and 64-byte cache-line alignment yield further reductions in L1 cache misses)._

## 🧠 Architectural Design

In high-frequency trading, OS context switches and CPU cache misses are fatal. This engine strictly adheres to hardware-aware engineering principles:

### 1. Zero Dynamic Allocation (Custom Memory Pool)

Dynamic memory allocation (`new`/`delete`) requires locks and OS interaction. This engine pre-allocates a massive contiguous block of `Order` objects at startup. During runtime, it uses a custom, lock-free stack allocator to claim and recycle memory in strict $O(1)$ time.

### 2. Intrusive Doubly-Linked Lists

Standard linked lists (like `std::list`) wrap data in nodes, destroying memory contiguity and causing cache misses. This architecture uses an **intrusive** design: the `next` and `prev` pointers are stored directly inside the `alignas(64)` Order struct. This ensures the pointers and the trading data are loaded into the L1 CPU cache simultaneously.

### 3. $O(1)$ Flat Array Price Levels

Instead of maintaining an $O(\log n)$ Red-Black tree (`std::map`) for price levels, the order book utilizes a flat array where the index represents the normalized price tick.

- **Insertion:** $O(1)$ memory grab + $O(1)$ array lookup + $O(1)$ tail append.
- **Cancellation:** $O(1)$ hash map address lookup + $O(1)$ intrusive unlink + $O(1)$ memory recycle.

## 🚀 Getting Started

### Prerequisites

- C++20 compliant compiler (`GCC`, `Clang`, or `Apple Clang`)
- `CMake` (>= 3.14)
- `GoogleTest` and `Google Benchmark`

### Build Instructions

The project uses an out-of-source CMake build sequence.

```bash
# Clone the repository
git clone [https://github.com/KrishnaChamarthy/limit-order-book.git](https://github.com/KrishnaChamarthy/limit-order-book.git)
cd limit-order-book

# Generate the build system (Ninja recommended for speed)
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release

# Compile the engine, tests, and benchmarks
cmake --build build

```

### Running Tests & Benchmarks

Verify correctness and memory safety:

```bash
./build/tests/run_tests

```

Measure your machine's hardware latency:

```bash
./build/benchmarks/run_benchmarks

```

## 🛠️ Future Optimizations (Roadmap)

- Replace `std::unordered_map` with a custom Robin Hood hash table for denser cache locality during order cancellations.
- Implement `__builtin_prefetch` instructions to pull upcoming memory pool nodes into the L1 cache before the CPU pipeline demands them.
- Wrap the engine in a kernel-bypass UDP network socket (e.g., Solarflare / OpenOnload) to ingest standard ITCH market data.

```

```
