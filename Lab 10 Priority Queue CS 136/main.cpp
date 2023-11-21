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
TABLE_SIZE = SETW_AMOUNT + SETW_TRACKING_NUM + SETW_RUSH_STATUS + SETW_AMOUNT_ORDERED + SETW_MARKUP_P + SETW_COST_TO_CUSTOMER + SETW_COST_TO_WAREHOUSE + 1,
SETW_SUMMARY = 25,
SETW_NUM_ORDERS = 16,
SETW_COST_TO_MAKE = 16,
SETW_NET_PROFIT = 14,
SETW_GROSS_PROFIT = 16,
SET_INVENTORY_LEFT = 18,
SUMMARY_TABLE_SIZE = SETW_NUM_ORDERS+SETW_COST_TO_MAKE+SETW_NET_PROFIT+SETW_GROSS_PROFIT+SET_INVENTORY_LEFT;

const double
WIDGET_PRICE = 5.00;

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

void PrintSummary(int totalOrders, double totalCostToMake, double totalProfit, double totalCostToCutomer, int quantityOnHand, string summaryBorder);

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
        " widgets in the queue\n" 
        << quantityOnHand << " more widgets can be proccessed today\n";
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

        try {
            pQ.Enqueue(order);
        }
        catch (FullPQ&) {
            cerr << "Queue is full, cannot process order\n\n";
        }
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
    stringstream summaryBorder("");
    summaryBorder << setfill('-') << setw(SUMMARY_TABLE_SIZE) << "\n";
    string sBorder = summaryBorder.str();
    if (!pQ.IsEmpty()) {
        cout << "\n" << header << border;
        while (!pQ.IsEmpty()) {
            Order order;
            try {
                pQ.Dequeue(order);
            }
            catch (EmptyPQ&) {
                cerr << "Unable to process order from an empty queue\n\n";
            }
            CalculateCosts(order, totalCostToMake, totalProfit, totalCostCustomer, totalOrders);
        }
        cout << border;
        cout << right << "\n\nDAY " << day << " SUMMARY: \n\n";
        PrintSummary(totalOrders, totalCostToMake, totalProfit, totalCostCustomer, quantityOnHand, sBorder);
    }
    else
        cout << "No orders processed today\n";
    day++;
    trackingNumber = 0;
    quantityOnHand = MAX_NUM_WIDGETS_PER_DAY;
}

void CalculateCosts(Order order, double& totalCostWarehouse, double& totalProfit, double& totalCostCustomer, int& totalAmountOrdered) {
    totalAmountOrdered += order.amountOrdered;
    double orderCostToMake = order.amountOrdered * WIDGET_PRICE;
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


void PrintSummary(int totalOrders, double totalCostToMake, double totalProfit, double totalCostCustomer, int quantityOnHand, string summaryBorder) {
    cout << setw(SETW_NUM_ORDERS) << "Number of orders" << 
        setw(SETW_COST_TO_MAKE) << "Cost to Make" << 
        setw(SETW_NET_PROFIT) << "Net Profit" << 
        setw(SETW_GROSS_PROFIT) << "Gross Profit" << 
        setw(SET_INVENTORY_LEFT) << "Inventory left\n" << 
        summaryBorder << right << 
        setw(SETW_NUM_ORDERS) << totalOrders << 
        setw(SETW_COST_TO_MAKE) << totalCostToMake << 
        setw(SETW_NET_PROFIT) << totalProfit << 
        setw(SETW_GROSS_PROFIT) << totalCostCustomer << 
        setw(SET_INVENTORY_LEFT - 1) << quantityOnHand << 
        "\n" << summaryBorder;
}
