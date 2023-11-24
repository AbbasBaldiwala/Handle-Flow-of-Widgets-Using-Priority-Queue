#ifndef HEAP_H
#define HEAP_H

#include "Order.h"
struct Heap {
	void ReheapDown(int root, int bottom); //moves node to correct spot
	void ReheapUp(int root, int bottom); //moves node to correct spot
	void Swap(Order& , Order&); //swaps 2 nodes
	Order* orders; // Array to be allocated dynamically in PQ class
	int numOrders;
};

#endif