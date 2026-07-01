#pragma once
#include <cstdint>

namespace lob {

    struct alignas(64) Order {
        uint64_t order_id;
        uint64_t price_tick;
        uint32_t quantity;
        bool is_buy;

        Order* next = nullptr;
        Order* prev = nullptr;
    };
}