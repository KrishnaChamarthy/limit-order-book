#pragma once
#include "order.h"
#include <vector>
#include <cstddef>

namespace lob {

    class OrderPool {
        private: 
            std::vector<Order> pool;
            std::vector<Order*> free_list;

        public: 
            explicit OrderPool(size_t capacity){
                pool.resize(capacity);
                free_list.reserve(capacity);
                
                for (size_t i = capacity; i > 0; i--){
                    free_list.push_back(&pool[i - 1]);
                }
            }

            [[nodiscard]] inline Order* allocate() noexcept {
                if (free_list.empty()) [[unlikely]] {
                    return nullptr;
                } 

                Order* order = free_list.back();
                free_list.pop_back();
                return order;
            }

            inline void deallocate(Order* order) noexcept {
                order->next = nullptr;
                order->prev = nullptr;

                free_list.push_back(order);
            }
    };
    
}