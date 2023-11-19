#ifndef HEAP_H
#define HEAP_H

#include "Order.h"
struct Heap {
	void ReheapDown(int root, int bottom);
	void ReheapUp(int root, int bottom);
	void Swap(Order& , Order&);
	Order* orders; // Array to be allocated dynamically
	int numOrders;
};

#endif