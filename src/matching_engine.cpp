#include "matching_engine.h"
#include <algorithm>

namespace lob {

    void LimitOrderBook::cancel_order(uint64_t id) {
        auto it = order_map.find(id);
        if (it == order_map.end()) [[unlikely]] {
            return;
        }

        Order* order = it->second;

        if (order->is_buy){
            bids[order->price_tick].remove(order);
        } else {
            asks[order->price_tick].remove(order);
        }

        order_map.erase(it);
        pool.deallocate(order);
    }

    void LimitOrderBook::add_order(uint64_t id, bool is_buy, uint32_t price, uint32_t qty) {
        
        if (is_buy) {
            while(qty > 0 && price >= best_ask){
                PriceLevel& level = asks[best_ask];
                Order* resting = level.head;

                while(resting && qty > 0){
                    uint32_t fill_qty = std::min(qty, resting->quantity);
                    qty -= fill_qty;
                    resting->quantity -= fill_qty;

                    if (resting->quantity == 0){
                        Order* next_resting = resting->next;
                        level.remove(resting);
                        order_map.erase(resting->order_id);
                        pool.deallocate(resting);
                        resting = next_resting;
                    }
                }

                if (level.head == nullptr){
                    do {
                        best_ask++;
                    } while(best_ask < MAX_PRICE_TICKS && asks[best_ask].head == nullptr);
                }
            }

            if (qty > 0){
                Order* new_order = pool.allocate();
                if (!new_order) [[unlikely]] return;

                new_order->order_id = id;
                new_order->is_buy = true;
                new_order->price_tick = price;
                new_order->quantity = qty;

                bids[price].append(new_order);
                order_map[id] = new_order;

                if (price > best_bid){
                    best_bid = price;
                }
            }
        } else {
            while(qty > 0 && price <= best_bid){
                PriceLevel&  level = bids[best_bid];
                Order* resting = level.head;

                while(resting && qty > 0){
                    uint32_t fill_qty = std::min(qty, resting->quantity);
                    qty -= fill_qty;
                    resting->quantity -= fill_qty;

                    if (resting->quantity == 0){
                        Order* next_resting = resting->next;
                        level.remove(resting);
                        order_map.erase(resting->order_id);
                        pool.deallocate(resting);
                        resting = next_resting;
                    }
                }

                if (level.head == nullptr){
                    do {
                        if (best_bid == 0) break;
                        best_bid--;
                    } while (best_bid > 0 && bids[best_bid].head == nullptr);
                }
            }

            if (qty > 0){
                Order* new_order = pool.allocate();
                if (!new_order) [[unlikely]] return;

                new_order->order_id = id;
                new_order->is_buy = false;
                new_order->price_tick = price;
                new_order->quantity = qty;

                asks[price].append(new_order);
                order_map[id] = new_order;

                if (price < best_ask){
                    best_ask = price;
                }
            }
        }
        
    }

}