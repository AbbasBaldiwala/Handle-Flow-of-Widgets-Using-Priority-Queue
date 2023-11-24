#include"PriorityQueue.h"
#include <iostream>

PQ::PQ(int size) {
	//Pre: none
	//Post:queue is created
	SetPQ(size);
}


PQ::PQ() {
	//Pre: none
	//Post:queue is created
	SetPQ(DEFAULT_QUEUE_SIZE); //change
}

void PQ::SetPQ(int size) {
	//Pre: heap memory must exist
	//Post:queue is created
	try {
		maxItems = size;
		ordersHeap.orders = new Order[size];
		length = 0;
	}
	catch (std::bad_alloc error) {
		std::cerr << "Creation of Queue failed, " << error.what();
	}
}

void PQ::MakeEmpty() {
	//Pre: none
	//Post: queue is deleted
	length = 0;
	if (ordersHeap.orders != nullptr) {
		delete[] ordersHeap.orders;
		ordersHeap.orders = nullptr;
	}
}

bool PQ::IsEmpty() const {
	//Pre: none
	//Post: returns false if queue is empty
	return length == 0;
}

bool PQ::IsFull() const {
	//Pre: none
	//Post: returns true if queue is full
	return length == maxItems;
}

void PQ::Enqueue(Order newItem) {
	//Pre: none
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
	//Pre: none
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
	//Pre: other queue must exist
	//Post: a copy of the queue is created
	int size = other.maxItems;
	SetPQ(size);
	length = other.length;
	for (int i = 0; i < size; i++) {
		ordersHeap.orders[i] = other.ordersHeap.orders[i];
	}
}

PQ::~PQ() {
	//Pre: none
	//Post:queue is destroyed
	MakeEmpty();
}

PQ::PQ(const PQ& other) {
	//Pre: other queue must exist
	//Post: a copy of the queue is created
	try {
		copyPQ(other);
	}
	catch (std::bad_alloc error) {
		std::cout << "Copy Failed, " << error.what();
	}
}

PQ& PQ::operator=(const PQ& RHS) {
	//Pre: other queue must exist
	//Post: a copy of the queue is created
	PQ temp(*this); 

	try {
		if (this != &RHS && temp.ordersHeap.orders != nullptr) {
			MakeEmpty();
			copyPQ(RHS);
		}
	}
	catch (std::bad_alloc error) {
		MakeEmpty();
		copyPQ(temp); // original queue is saved
		std::cout << "Copy Failed, Original Queue intact, " << error.what();
	}
	return *this;
}