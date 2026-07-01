find_package(GTest REQUIRED)

add_executable(run_tests test_engine.cpp)

# Link the core library and Google Test
target_link_libraries(run_tests PRIVATE core_engine GTest::gtest_main)

# Register the tests with CTest
include(GoogleTest)
gtest_discover_tests(run_tests)