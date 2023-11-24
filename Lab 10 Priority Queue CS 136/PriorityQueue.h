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
	void SetPQ(int); //created the queue of the correct size
	void MakeEmpty(); // deletes the queue
	bool IsEmpty() const; //returns true if the queue is empty
	bool IsFull() const; //returns true if the queue is full
	void Enqueue(Order order); // adds an order to the queue
	void Dequeue(Order& order); // removes an order from the queue and copies it into the parameter
	
	~PQ();
	PQ(const PQ&);
	PQ& operator=(const PQ&);
private:
	int length;
	Heap ordersHeap;
	int maxItems;

	void copyPQ(const PQ& other); //copys the queue
};


#endif
