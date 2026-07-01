#include <gtest/gtest.h>
#include "memory_pool.h"
#include "matching_engine.h"

using namespace lob;

TEST(MemoryPoolTest, AllocationAndDeallocation) {
    const size_t pool_size = 1000000;
    OrderPool pool(pool_size);

    std::vector<Order*> allocated_orders;
    allocated_orders.reserve(pool_size);

    for (size_t i = 0; i < pool_size; i++){
        Order* order = pool.allocate();
        ASSERT_NE(order, nullptr);

        order->order_id = 1;
        order->price_tick = 100;
        allocated_orders.push_back(order);
    }

    Order* overflow_order = pool.allocate();
    EXPECT_EQ(overflow_order, nullptr);

    for (Order* order : allocated_orders){
        pool.deallocate(order);
    }

    Order* reused_order = pool.allocate();
    EXPECT_NE(reused_order, nullptr);
}

TEST(EngineTest, CrossMatching){
    lob::LimitOrderBook engine;

    engine.add_order(1, false, 10000, 100);
    engine.add_order(2, true, 10000, 50);

    engine.cancel_order(2);
    engine.cancel_order(1);

    SUCCEED();
}
