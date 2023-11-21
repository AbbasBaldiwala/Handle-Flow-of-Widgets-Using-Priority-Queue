#include"PriorityQueue.h"
#include <iostream>

PQ::PQ(int size) {
	SetPQ(size);
}


PQ::PQ() {
	SetPQ(DEFAULT_QUEUE_SIZE); //change
}

void PQ::SetPQ(int size) {
	maxItems = size;
	ordersHeap.orders = new Order[size];
	length = 0;
}

void PQ::MakeEmpty() {
	length = 0;
	if (ordersHeap.orders != nullptr) {
		delete[] ordersHeap.orders;
		ordersHeap.orders = nullptr;
	}
}

bool PQ::IsEmpty() const {
	return length == 0;
}

bool PQ::IsFull() const {
	return length == maxItems;
}

void PQ::Enqueue(Order newItem) {
	// Post: newItem is in the queue
	if (length == maxItems)
		throw FullPQ();
	else {
		length++;
		ordersHeap.orders[length - 1] = newItem;
		ordersHeap.ReheapUp(0, length - 1);
	}
}

void PQ::Dequeue(Order& item) {
	// Post: element with highest priority has been removed
	// from the queue; a copy is returned in item
	if (length == 0)
		throw EmptyPQ();
	else {
		item = ordersHeap.orders[0];
		ordersHeap.orders[0] = ordersHeap.orders[length - 1];
		length--;
		ordersHeap.ReheapDown(0, length - 1);
	}
}


void PQ::copyPQ(const PQ& other) {
	int size = other.maxItems;
	SetPQ(size);
	length = other.length;
	for (int i = 0; i < size; i++) {
		ordersHeap.orders[i] = other.ordersHeap.orders[i];
	}
}

PQ::~PQ() {
	MakeEmpty();
}

PQ::PQ(const PQ& other) {
	try {
		copyPQ(other);
	}
	catch (std::bad_alloc error) {
		std::cout << "Copy Failed, " << error.what();
	}
}

PQ& PQ::operator=(const PQ& RHS) {
	try {
		if (this != &RHS) {
			MakeEmpty();
			copyPQ(RHS);
		}
	}
	catch (std::bad_alloc error) {
		std::cout << "Copy Failed, " << error.what();
	}
	return *this;
}