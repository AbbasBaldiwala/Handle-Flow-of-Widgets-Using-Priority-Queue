#include <iostream>
#include <iomanip>
#include <sstream>
#include "PriorityQueue.h"

using namespace std;

const int 
MAX_NUM_WIDGETS_PER_DAY = 100,
DISPLAY_PRECISION = 2,
MAX_QUEUE_SIZE = 10,
BASE_TRACKING_NUMBER = 100, 
SETW_AMOUNT = 10,
SETW_TRACKING_NUM = 15, 
SETW_RUSH_STATUS = 18, 
SETW_AMOUNT_ORDERED = 15, 
SETW_MARKUP_P = 15,
SETW_COST_TO_CUSTOMER = 15, 
SETW_COST_TO_WAREHOUSE = 15,
TABLE_SIZE = SETW_AMOUNT+SETW_TRACKING_NUM+SETW_RUSH_STATUS+SETW_AMOUNT_ORDERED+SETW_MARKUP_P+SETW_COST_TO_CUSTOMER+SETW_COST_TO_WAREHOUSE + 1;

const double 
WIDGET_PRICE = 5.00, 
WIDGET_COST_TO_MAKE = 1.50;

enum Menu{DISPLAY_INVENTORY_ON_HAND = 1, PLACE_ORDER, CLOSE_FOR_THE_DAY, QUIT};

//enum RushStatus{EXTREME = 1, EXPEDITE, STANDARD, QUIT};


/*
pre: none
post: clears cin, clears the keyboard buffer, prints an error message
*/
void ClearInvalidInput(string errMsg);

void DisplayInventoryOnHand(const int& quantityOnHand);

/*pre:
post:*/
void EnterPlaceOrderSubmenu(int& quantityOnHand, PQ& pQ, int& trackingNum, int day);

/*pre:
post:*/
int GetAmountOrdered(int& quantityOnHand);

/*pre:
post:*/
int GetRushStatus();

/*pre:
post:*/
void CloseWarehouse(PQ& pQ, int& day, int& trackingNumber, int& quantityOnHand, string header, string border);

/*pre:
post:*/
void CalculateCosts(Order order, double& totalCostWarehouse, double& totalProfit, double& totalCostCustomer, int& totalAmountOrdered);

/*pre:
post:*/
void PrintOrder(Order order, double costToCustomer, double costToWarehouse);

int main() {
    int userChoice,
        quantityOnHand = MAX_NUM_WIDGETS_PER_DAY,
        trackingNum = 0,
        day = 1;
    stringstream headerSS(""), borderSS("");
    borderSS << setfill('-') << setw(TABLE_SIZE) << "\n";
    headerSS << right << setw(SETW_AMOUNT) << "Tracking #" << setw(SETW_TRACKING_NUM) <<
        "Rush Status" << setw(SETW_RUSH_STATUS) <<
        "Amount Ordered" << setw(SETW_AMOUNT_ORDERED) <<
        "% Markup" << setw(SETW_MARKUP_P) <<
        "Price" << setw(SETW_COST_TO_CUSTOMER) <<
        "Cost" << setw(SETW_COST_TO_WAREHOUSE + 1) <<
        "Total Markup\n";
    string header = headerSS.str(), border = borderSS.str();
    cout << fixed << setprecision(DISPLAY_PRECISION);
    PQ pQ(MAX_QUEUE_SIZE);
	do {
        cout << "\n\nMENU: \n" <<
            DISPLAY_INVENTORY_ON_HAND << ". DSIPLAY INVENTORY ON HAND\n" <<  //display how many more orders can take?
            PLACE_ORDER << ". PLACE NEW ORDER\n" <<
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
            CloseWarehouse(pQ, day, trackingNum, quantityOnHand, header, border);
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
    if (quantityOnHand > 0 && !pQ.IsFull()) {
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

void CloseWarehouse(PQ& pQ, int& day, int& trackingNumber, int& quantityOnHand, string header, string border) {
    double totalCostToMake = 0, totalProfit = 0, totalCostCustomer = 0;
    int totalOrders = 0;
    cout << header << border;
    while (!pQ.IsEmpty()) {
        Order order;
        pQ.Dequeue(order);
        CalculateCosts(order, totalCostToMake, totalProfit, totalCostCustomer, totalOrders);
    }
    cout << border << "Day " << day << " Totals: \n"
        "Orders Processed: " << totalOrders << "\n" <<
        "Cost To Warehouse: " << totalCostToMake << "\n" <<
        "Net Profit: " << totalProfit << "\n" <<
        "Total Cost To Customers: " << totalCostCustomer << "\n";
    day++;
    trackingNumber = 0;
    quantityOnHand = MAX_NUM_WIDGETS_PER_DAY;
}

void CalculateCosts(Order order, double& totalCostWarehouse, double& totalProfit, double& totalCostCustomer, int& totalAmountOrdered) {
    totalAmountOrdered += order.amountOrdered;
    double orderCostToMake = order.amountOrdered * WIDGET_COST_TO_MAKE;
    double grossProfit = order.amountOrdered * WIDGET_PRICE * (1 + (order.percentMarkup / 100));
    double netProfit = grossProfit - orderCostToMake;
    PrintOrder(order, grossProfit, orderCostToMake);
    totalCostWarehouse += orderCostToMake;
    totalProfit += netProfit;
    totalCostCustomer += grossProfit;
}

void PrintOrder(Order order, double costToCustomer, double costToWarehouse) {
    cout << right << setw(SETW_AMOUNT) << order.trackingNumber << setw(SETW_TRACKING_NUM);
    if (order.rushStatus == STANDARD)
        cout << "STANDARD" << setw(SETW_RUSH_STATUS);
    else if (order.rushStatus == EXPEDITE)
        cout << "EXPEDITE" << setw(SETW_RUSH_STATUS);
    else
        cout << "EXTREME" << setw(SETW_RUSH_STATUS);
    cout << right << order.amountOrdered << setw(SETW_AMOUNT_ORDERED) <<
        order.percentMarkup << setw(SETW_MARKUP_P) <<
        costToCustomer << setw(SETW_COST_TO_CUSTOMER) <<
        costToWarehouse << setw(SETW_COST_TO_WAREHOUSE) <<
        (order.amountOrdered * WIDGET_PRICE) * (order.percentMarkup / 100) << "\n";

}

