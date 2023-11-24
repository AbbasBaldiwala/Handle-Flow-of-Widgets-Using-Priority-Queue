#include "Order.h"

void Order::SetOrder(int rushStat, int trackingNum, int amount) {
	//Pre: none
	//Post: all the correct values are placed in the order
	rushStatus = rushStat;
	trackingNumber = trackingNum;
	amountOrdered = amount;
	switch (rushStat)
	{
	case STANDARD:
		percentMarkup = STANDARD_MARKUP;
		break;
	case EXPEDITE:
		percentMarkup = EXPEDITE_MARKUP;
		break;
	case EXTREME:
		percentMarkup = EXTREME_MARKUP;
		break;
	default:
		percentMarkup = -1;
		break;
	}
}

Order::Order() {
	//Pre: none
	//Post: an order is created
	SetOrder(-1, -1, -1);
}

Order::Order(int rushStat, int trackingNum, int amount) {
	//Pre: none
	//Post: an order is created
	SetOrder(rushStat, trackingNum, amount);
}