#include <iostream>
#include "PriorityQueue.h"

using namespace std;

const int MAX_NUM_WIDGETS_PER_DAY = 100, DISPLAY_PRECISION = 2,
MAX_QUEUE_SIZE = 10, BASE_TRACKING_NUMBER = 100;

const double WIDGET_COST = 5.00;

enum Menu{DISPLAY_INVENTORY_ON_HAND = 1, PLACE_ORDER, CLOSE_FOR_THE_DAY, QUIT};

//enum RushStatus{EXTREME = 1, EXPEDITE, STANDARD, QUIT};


/*
pre: none
post: clears cin, clears the keyboard buffer, prints an error message
*/
void ClearInvalidInput(string errMsg);

void DisplayInventoryOnHand(const int& quantityOnHand);

/**/
void EnterPlaceOrderSubmenu(int& quantityOnHand, PQ& pQ, int& trackingNum, int day);

int GetAmountOrdered(int& quantityOnHand);

int GetRushStatus();

void CloseWarehouse(PQ& pQ);

int main() {
    int userChoice,
        quantityOnHand = MAX_NUM_WIDGETS_PER_DAY,
        trackingNum = 0,
        day = 1;
    PQ pQ(MAX_QUEUE_SIZE);
	do {
        cout << "\n\nMENU: \n" <<
            DISPLAY_INVENTORY_ON_HAND << ". DSIPLAY INVENTORY ON HAND\n" <<
            PLACE_ORDER << ". ACCEPT NEW ORDER\n" <<
            CLOSE_FOR_THE_DAY << ". CLOSE FOR THE DAY\n" <<
            QUIT << ". QUIT\n\n";
        cin >> userChoice;
        switch (userChoice) {
        case DISPLAY_INVENTORY_ON_HAND:
            DisplayInventoryOnHand(quantityOnHand);
            break;
        case PLACE_ORDER:
            EnterPlaceOrderSubmenu(quantityOnHand, pQ, trackingNum, day);
            break;
        case CLOSE_FOR_THE_DAY:
            CloseWarehouse(pQ);
            break;
        case QUIT:
            cout << "QUITTING..." << endl;
            break;
        default:
            ClearInvalidInput("INVALID MENU SELECTION");
        }
	} while (userChoice != QUIT);
	return 0;
}


void ClearInvalidInput(string errMsg) {
    cout << "\n" << errMsg << "\n";
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void DisplayInventoryOnHand(const int& quantityOnHand) {
    cout << "The warehouse currently has " << MAX_NUM_WIDGETS_PER_DAY - quantityOnHand << 
        " widgets in the queue\nThe warehouse can process " 
        << quantityOnHand << " more widgets today\n";
}


void EnterPlaceOrderSubmenu(int& quantityOnHand, PQ& pQ, int& trackingNum, int day) {
    if (quantityOnHand > 0) {
        int numOrdered, rushStatus;
        Order order;
        cout << "How many widgets would you like to place an order for: \n";
        numOrdered = GetAmountOrdered(quantityOnHand);
        cout << "Select a Rush Status for your " << numOrdered << " widget order:\n" <<
            STANDARD << ". Standard (" << STANDARD_MARKUP << "% markup)\n" <<
            EXPEDITE << ". Expedite (" << EXPEDITE_MARKUP << "% markup)\n" <<
            EXTREME << ". Extreme (" << EXTREME_MARKUP << "% markup)\n\n";
        rushStatus = GetRushStatus();
        int trackingNumber = (BASE_TRACKING_NUMBER * day) + (++trackingNum);
        order.SetOrder(rushStatus, trackingNumber, numOrdered);

        pQ.Enqueue(order);
    }
    else {
        cout << "The warehouse cannot process any more orders for the day.\n";
    }
}

int GetAmountOrdered(int& quantityOnHand) {
    int numOrdered;
    bool invalidInput;
    do {
        invalidInput = false;
        cin >> numOrdered;
        if (!numOrdered || numOrdered <= 0) {
            ClearInvalidInput("AMOUNT ORDERED MUST BE A VALID NUMBER GREATER THAN 0\nRE-ENTER:");
            invalidInput = true;
        }
        else if (!(quantityOnHand - numOrdered >= 0)) {
            ClearInvalidInput("NOT ENOUGH INVENTORY\nRE-ENTER:");
            invalidInput = true;
        }
        else {
            quantityOnHand -= numOrdered;
        }
    } while (invalidInput);
    return numOrdered;
}

int GetRushStatus() {
    int rushStatus;
    bool invalidInput;
    do {
        invalidInput = false;
        cin >> rushStatus;
        if (!(rushStatus == STANDARD || rushStatus == EXPEDITE || rushStatus == EXTREME)) {
            ClearInvalidInput("INVALID INPUT\nRE-ENTER:");
            invalidInput = true;
        }
    } while (invalidInput);
    return rushStatus;
}

void CloseWarehouse(PQ& pQ) {
    while (!pQ.IsEmpty()) {
        Order order;
        pQ.Dequeue(order);

        cout << "order rush stat: " << order.rushStatus << endl;
        cout << "order quanitity: " << order.amountOrdered << endl;
        cout << "order markup: " << order.percentMarkup << endl;
        cout << "order tracking num: " << order.trackingNumber << endl;
    }
}