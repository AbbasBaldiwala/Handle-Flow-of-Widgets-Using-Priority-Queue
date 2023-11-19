#include "Heap.h"

void Heap::ReheapDown(int root, int bottom) {
	int maxChild; int rightChild; int leftChild;
	leftChild = root * 2 + 1;
	rightChild = root * 2 + 2;
	int leftRushStatus = orders[leftChild].rushStatus,
		rightRushStatus = orders[rightChild].rushStatus;
	if (leftChild <= bottom) {
		if (leftChild == bottom)
			maxChild = leftChild;
		else {
			if (leftRushStatus > rightRushStatus|| leftRushStatus == rightRushStatus 
				&& orders[leftChild].trackingNumber < orders[rightChild].trackingNumber)
				maxChild = leftChild;
			else
				maxChild = rightChild;
		}
		if (orders[root].rushStatus < orders[maxChild].rushStatus || orders[root].rushStatus == orders[maxChild].rushStatus
			&& orders[root].trackingNumber > orders[maxChild].trackingNumber) {
			Swap(orders[root], orders[maxChild]);
			ReheapDown(maxChild, bottom);
		}
	}
}

void Heap::ReheapUp(int root, int bottom) {
	// Post: Heap property is restored.
	int parent;
	if (bottom > root) {
		parent = (bottom - 1) / 2;
		if (orders[parent].rushStatus < orders[bottom].rushStatus || orders[parent].rushStatus == orders[bottom].rushStatus 
			&& orders[parent].trackingNumber > orders[bottom].trackingNumber) {
			Swap(orders[parent], orders[bottom]);
			ReheapUp(root, parent);
		}
	}
}

void Heap::Swap(Order& upper, Order& lower) {
	Order temp = lower;
	lower = upper;
	upper = temp;
}