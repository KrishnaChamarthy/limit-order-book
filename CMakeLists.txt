cmake_minimum_required(VERSION 3.14)
project(LimitOrderBook VERSION 1.0 LANGUAGES CXX)

# Force C++20 for advanced features ([[likely]], concepts, etc.)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Aggressive optimization flags for HFT
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -flto -DNDEBUG")

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Create the core library
add_library(core_engine 
    src/memory_pool.cpp 
    src/matching_engine.cpp
)

# The main executable
add_executable(lob_app src/main.cpp)
target_link_libraries(lob_app PRIVATE core_engine)