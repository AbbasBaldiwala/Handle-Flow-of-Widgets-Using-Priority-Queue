#ifndef ORDER_H
#define ORDER_H

//change markup percent
const double STANDARD_MARKUP = 33, EXPEDITE_MARKUP = 55, EXTREME_MARKUP = 77;

enum RushStatus{STANDARD = 1, EXPEDITE, EXTREME};

class Order {
public:
    int rushStatus;
    int trackingNumber;
    int amountOrdered;
    double percentMarkup;

    Order();
    Order(int, int, int);

    void SetOrder(int, int, int); //populates the order
};


#endif
