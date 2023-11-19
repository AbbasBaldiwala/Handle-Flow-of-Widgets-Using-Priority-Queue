#include "Order.h"

void Order::SetOrder(int rushStat, int trackingNum, int amount) {
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
	SetOrder(-1, -1, -1);
}

Order::Order(int rushStat, int trackingNum, int amount) {
	SetOrder(rushStat, trackingNum, amount);
}