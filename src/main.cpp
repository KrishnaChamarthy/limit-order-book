#include <iostream>
#include "memory_pool.h"

int main() {
    std::cout << "Ultra-Low Latency Limit Order Book Initialized.\n";

    lob::OrderPool pool(1000);
    lob::Order* order = pool.allocate();

    if (order) {
        std::cout << "Successfully allocated memory pool.\n";
    }

    return 0;
}