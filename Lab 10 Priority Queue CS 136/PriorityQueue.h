#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "Heap.h"

const int DEFAULT_QUEUE_SIZE = 25;

class FullPQ {};
class EmptyPQ {};

class PQ {
public:
	PQ(int);
	PQ();
	void SetPQ(int);
	void MakeEmpty();
	bool IsEmpty() const;
	bool IsFull() const;
	void Enqueue(Order order);
	void Dequeue(Order& order);
	
	~PQ();
	PQ(const PQ&);
	PQ& operator=(const PQ&);
private:
	int length;
	Heap ordersHeap;
	int maxItems;

	void copyPQ(const PQ& other);
};


#endif
