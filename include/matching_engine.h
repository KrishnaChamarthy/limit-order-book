#pragma once
#include "order.h"
#include "memory_pool.h"
#include <array>
#include <unordered_map>
#include <iostream>

namespace lob {

    constexpr uint32_t MAX_PRICE_TICKS = 100000;

    struct PriceLevel {
        Order* head = nullptr;
        Order* tail = nullptr;

        inline void append(Order* order) noexcept {
            order->next = nullptr;
            order->prev = tail;

            if (tail){
                tail->next = order;
            } else {
                head = order;
            }

            tail = order;
        }

        inline void remove(Order* order) noexcept {
            if (order->prev){
                order->prev->next = order->next;
            } else {
                head = order->next;
            }
            if (order->next){
                order->next->prev = order->prev;
            } else {
                tail = order->prev;
            }
        }
    };

    class LimitOrderBook {
        private:
            std::array<PriceLevel, MAX_PRICE_TICKS> bids{};
            std::array<PriceLevel, MAX_PRICE_TICKS> asks{};
            
            uint32_t best_bid = 0;
            uint32_t best_ask = MAX_PRICE_TICKS - 1;

            std::unordered_map<uint64_t, Order*> order_map;

            OrderPool pool;

        public:
            explicit LimitOrderBook(size_t capacity = 1000000) : pool(capacity){
                order_map.reserve(capacity);
            }

            void add_order(uint64_t id, bool is_buy, uint32_t price, uint32_t qty);
            void cancel_order(uint64_t id);
    };

}