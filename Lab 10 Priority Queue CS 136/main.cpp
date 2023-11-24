#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits>
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
SETW_NUM_ORDERS = 16,
SETW_COST_TO_MAKE = 16,
SETW_NET_PROFIT = 14,
SETW_GROSS_PROFIT = 16,
SET_INVENTORY_LEFT = 18,
SUMMARY_TABLE_SIZE = SETW_NUM_ORDERS+SETW_COST_TO_MAKE+SETW_NET_PROFIT+SETW_GROSS_PROFIT+SET_INVENTORY_LEFT;

const double
WIDGET_PRICE = 5.00;

enum Menu{DISPLAY_INVENTORY_ON_HAND = 1, PLACE_ORDER, CLOSE_FOR_THE_DAY, QUIT};

//*"Gross Profit" in summary is the same as "Cost to Customer"

//Description: Clears invalid input
/*pre: none
post: clears cin, clears the keyboard buffer, prints an error message*/
void ClearInvalidInput(string errMsg);

//Description: Displays inventory
/*pre: none
post: Displays how many widgets and orders the warehouse has processed and can still process*/
void DisplayInventoryOnHand(const int& quantityOnHand, const int& spaceInQueue);

//Description: Allows user to place new order
/*pre: none
post: places a new order in the warehouse queue*/
void PlaceNewOrder(int& quantityOnHand, PQ& pQ, int& trackingNum, int day, int& spaceInQueue);

//Description: gets order amount from the user
/*pre:PlaceNewOrder function has been called
post: gets how many widgets the user wants*/
int GetAmountOrdered(int& quantityOnHand);

//Description: gets the rush status for the order from user
/*pre: PlaceNewOrder function has been called
post: gets the rush status of the order*/
int GetRushStatus();

//Description:closes the warehouse for the day
/*pre: none
post: Displays what the warehouse processed for the day*/
void CloseWarehouse(PQ& pQ, int& day, int& trackingNumber, int& quantityOnHand, string header, string border, int& spaceInQueue);

//Description: calculated the total costs and profits for the warehouse
/*pre: CloseWarehouse function called
post: Calculates how much the warehouse spent and made*/
void CalculateCosts(Order order, double& totalCostWarehouse, double& totalProfit, double& totalCostCustomer, int& totalAmountOrdered);

//Description:Prints the order
/*pre: CloseWarehouse function has been called
post: order is printed in a table format*/
void PrintOrder(Order order, double costToCustomer, double costToWarehouse);

//Description: Prints the days summary
/*pre: CloseWarehouse function has been called
post: all the days totals are printed in a table*/
void PrintSummary(int totalOrders, double totalCostToMake, double totalProfit, double totalCostToCutomer, int quantityOnHand, string summaryBorder);

int main() {
    int userChoice,
        quantityOnHand = MAX_NUM_WIDGETS_PER_DAY,
        spaceInQueue = MAX_QUEUE_SIZE,
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
            DisplayInventoryOnHand(quantityOnHand, spaceInQueue);
            break;
        case PLACE_ORDER:
            PlaceNewOrder(quantityOnHand, pQ, trackingNum, day, spaceInQueue);
            break;
        case CLOSE_FOR_THE_DAY:
            CloseWarehouse(pQ, day, trackingNum, quantityOnHand, header, border, spaceInQueue);
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

void DisplayInventoryOnHand(const int& quantityOnHand, const int& spaceInQueue) {
    cout << "The warehouse currently has " << MAX_NUM_WIDGETS_PER_DAY - quantityOnHand << 
        " widgets in the queue\n" 
        "There can be " << spaceInQueue << " more orders placed today\n"
        << quantityOnHand << " more widgets can be proccessed today\n";
}


void PlaceNewOrder(int& quantityOnHand, PQ& pQ, int& trackingNum, int day, int& spaceInQueue) {
    if (quantityOnHand > 0 && spaceInQueue > 0) {
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
            spaceInQueue--;
        }
        catch (FullPQ&) {
            cerr << "Queue is full, cannot process your order. Try again tomorrow\n\n";
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

void CloseWarehouse(PQ& pQ, int& day, int& trackingNumber, int& quantityOnHand, string header, string border, int& spaceInQueue) {
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
    spaceInQueue = MAX_QUEUE_SIZE;
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


//OUTPUT TESTS

//TEST 1 (successful orders placed)
/*
MENU:
1. DSIPLAY INVENTORY ON HAND
2. PLACE NEW ORDER
3. CLOSE FOR THE DAY
4. QUIT

1
The warehouse currently has 0 widgets in the queue
There can be 10 more orders placed today
100 more widgets can be proccessed today


MENU:
1. DSIPLAY INVENTORY ON HAND
2. PLACE NEW ORDER
3. CLOSE FOR THE DAY
4. QUIT

2
How many widgets would you like to place an order for:
20
Select a Rush Status for your 20 widget order:
1. Standard (33% markup)
2. Expedite (55% markup)
3. Extreme (77% markup)

1


MENU:
1. DSIPLAY INVENTORY ON HAND
2. PLACE NEW ORDER
3. CLOSE FOR THE DAY
4. QUIT

2
How many widgets would you like to place an order for:
4
Select a Rush Status for your 4 widget order:
1. Standard (33% markup)
2. Expedite (55% markup)
3. Extreme (77% markup)

3


MENU:
1. DSIPLAY INVENTORY ON HAND
2. PLACE NEW ORDER
3. CLOSE FOR THE DAY
4. QUIT

2
How many widgets would you like to place an order for:
10
Select a Rush Status for your 10 widget order:
1. Standard (33% markup)
2. Expedite (55% markup)
3. Extreme (77% markup)

2


MENU:
1. DSIPLAY INVENTORY ON HAND
2. PLACE NEW ORDER
3. CLOSE FOR THE DAY
4. QUIT

2
How many widgets would you like to place an order for:
15
Select a Rush Status for your 15 widget order:
1. Standard (33% markup)
2. Expedite (55% markup)
3. Extreme (77% markup)

1


MENU:
1. DSIPLAY INVENTORY ON HAND
2. PLACE NEW ORDER
3. CLOSE FOR THE DAY
4. QUIT

2
How many widgets would you like to place an order for:
5
Select a Rush Status for your 5 widget order:
1. Standard (33% markup)
2. Expedite (55% markup)
3. Extreme (77% markup)

3


MENU:
1. DSIPLAY INVENTORY ON HAND
2. PLACE NEW ORDER
3. CLOSE FOR THE DAY
4. QUIT

2
How many widgets would you like to place an order for:
7
Select a Rush Status for your 7 widget order:
1. Standard (33% markup)
2. Expedite (55% markup)
3. Extreme (77% markup)

2


MENU:
1. DSIPLAY INVENTORY ON HAND
2. PLACE NEW ORDER
3. CLOSE FOR THE DAY
4. QUIT

2
How many widgets would you like to place an order for:
5
Select a Rush Status for your 5 widget order:
1. Standard (33% markup)
2. Expedite (55% markup)
3. Extreme (77% markup)

1


MENU:
1. DSIPLAY INVENTORY ON HAND
2. PLACE NEW ORDER
3. CLOSE FOR THE DAY
4. QUIT

2
How many widgets would you like to place an order for:
9
Select a Rush Status for your 9 widget order:
1. Standard (33% markup)
2. Expedite (55% markup)
3. Extreme (77% markup)

1


MENU:
1. DSIPLAY INVENTORY ON HAND
2. PLACE NEW ORDER
3. CLOSE FOR THE DAY
4. QUIT

1
The warehouse currently has 75 widgets in the queue
There can be 2 more orders placed today
25 more widgets can be proccessed today


MENU:
1. DSIPLAY INVENTORY ON HAND
2. PLACE NEW ORDER
3. CLOSE FOR THE DAY
4. QUIT

2
How many widgets would you like to place an order for:
2
Select a Rush Status for your 2 widget order:
1. Standard (33% markup)
2. Expedite (55% markup)
3. Extreme (77% markup)

2


MENU:
1. DSIPLAY INVENTORY ON HAND
2. PLACE NEW ORDER
3. CLOSE FOR THE DAY
4. QUIT

3

Tracking #    Rush Status    Amount Ordered       % Markup          Price           Cost   Total Markup
-------------------------------------------------------------------------------------------------------
       102        EXTREME                 4          77.00          35.40          20.00          15.40
       105        EXTREME                 5          77.00          44.25          25.00          19.25
       103       EXPEDITE                10          55.00          77.50          50.00          27.50
       106       EXPEDITE                 7          55.00          54.25          35.00          19.25
       109       EXPEDITE                 2          55.00          15.50          10.00           5.50
       101       STANDARD                20          33.00         133.00         100.00          33.00
       104       STANDARD                15          33.00          99.75          75.00          24.75
       107       STANDARD                 5          33.00          33.25          25.00           8.25
       108       STANDARD                 9          33.00          59.85          45.00          14.85
-------------------------------------------------------------------------------------------------------


DAY 1 SUMMARY:

Number of orders    Cost to Make    Net Profit    Gross Profit   Inventory left
-------------------------------------------------------------------------------
              77          385.00        167.75          552.75               23
-------------------------------------------------------------------------------


MENU:
1. DSIPLAY INVENTORY ON HAND
2. PLACE NEW ORDER
3. CLOSE FOR THE DAY
4. QUIT

1
The warehouse currently has 0 widgets in the queue
There can be 10 more orders placed today
100 more widgets can be proccessed today


MENU:
1. DSIPLAY INVENTORY ON HAND
2. PLACE NEW ORDER
3. CLOSE FOR THE DAY
4. QUIT

2
How many widgets would you like to place an order for:
2
Select a Rush Status for your 2 widget order:
1. Standard (33% markup)
2. Expedite (55% markup)
3. Extreme (77% markup)

2


MENU:
1. DSIPLAY INVENTORY ON HAND
2. PLACE NEW ORDER
3. CLOSE FOR THE DAY
4. QUIT

2
How many widgets would you like to place an order for:
5
Select a Rush Status for your 5 widget order:
1. Standard (33% markup)
2. Expedite (55% markup)
3. Extreme (77% markup)

3


MENU:
1. DSIPLAY INVENTORY ON HAND
2. PLACE NEW ORDER
3. CLOSE FOR THE DAY
4. QUIT

2
How many widgets would you like to place an order for:
10
Select a Rush Status for your 10 widget order:
1. Standard (33% markup)
2. Expedite (55% markup)
3. Extreme (77% markup)

1


MENU:
1. DSIPLAY INVENTORY ON HAND
2. PLACE NEW ORDER
3. CLOSE FOR THE DAY
4. QUIT

2
How many widgets would you like to place an order for:
6
Select a Rush Status for your 6 widget order:
1. Standard (33% markup)
2. Expedite (55% markup)
3. Extreme (77% markup)

1


MENU:
1. DSIPLAY INVENTORY ON HAND
2. PLACE NEW ORDER
3. CLOSE FOR THE DAY
4. QUIT

3

Tracking #    Rush Status    Amount Ordered       % Markup          Price           Cost   Total Markup
-------------------------------------------------------------------------------------------------------
       202        EXTREME                 5          77.00          44.25          25.00          19.25
       201       EXPEDITE                 2          55.00          15.50          10.00           5.50
       203       STANDARD                10          33.00          66.50          50.00          16.50
       204       STANDARD                 6          33.00          39.90          30.00           9.90
-------------------------------------------------------------------------------------------------------


DAY 2 SUMMARY:

Number of orders    Cost to Make    Net Profit    Gross Profit   Inventory left
-------------------------------------------------------------------------------
              23          115.00         51.15          166.15               77
-------------------------------------------------------------------------------


MENU:
1. DSIPLAY INVENTORY ON HAND
2. PLACE NEW ORDER
3. CLOSE FOR THE DAY
4. QUIT

4
QUITTING...

*/

//TEST 2 (no orders placed)
/*

MENU:
1. DSIPLAY INVENTORY ON HAND
2. PLACE NEW ORDER
3. CLOSE FOR THE DAY
4. QUIT

1
The warehouse currently has 0 widgets in the queue
There can be 10 more orders placed today
100 more widgets can be proccessed today


MENU:
1. DSIPLAY INVENTORY ON HAND
2. PLACE NEW ORDER
3. CLOSE FOR THE DAY
4. QUIT

3
No orders processed today


MENU:
1. DSIPLAY INVENTORY ON HAND
2. PLACE NEW ORDER
3. CLOSE FOR THE DAY
4. QUIT

dafsf

INVALID MENU SELECTION


MENU:
1. DSIPLAY INVENTORY ON HAND
2. PLACE NEW ORDER
3. CLOSE FOR THE DAY
4. QUIT

23

INVALID MENU SELECTION


MENU:
1. DSIPLAY INVENTORY ON HAND
2. PLACE NEW ORDER
3. CLOSE FOR THE DAY
4. QUIT

4
QUITTING...
*/

//TEST 3 (invalid numbers entered by user)
/*

MENU:
1. DSIPLAY INVENTORY ON HAND
2. PLACE NEW ORDER
3. CLOSE FOR THE DAY
4. QUIT

2
How many widgets would you like to place an order for:
0

AMOUNT ORDERED MUST BE A VALID NUMBER GREATER THAN 0
RE-ENTER:
-1

AMOUNT ORDERED MUST BE A VALID NUMBER GREATER THAN 0
RE-ENTER:
adf

AMOUNT ORDERED MUST BE A VALID NUMBER GREATER THAN 0
RE-ENTER:
5
Select a Rush Status for your 5 widget order:
1. Standard (33% markup)
2. Expedite (55% markup)
3. Extreme (77% markup)

0

INVALID INPUT
RE-ENTER:
4

INVALID INPUT
RE-ENTER:
-1

INVALID INPUT
RE-ENTER:
2


MENU:
1. DSIPLAY INVENTORY ON HAND
2. PLACE NEW ORDER
3. CLOSE FOR THE DAY
4. QUIT

3

Tracking #    Rush Status    Amount Ordered       % Markup          Price           Cost   Total Markup
-------------------------------------------------------------------------------------------------------
       101       EXPEDITE                 5          55.00          38.75          25.00          13.75
-------------------------------------------------------------------------------------------------------


DAY 1 SUMMARY:

Number of orders    Cost to Make    Net Profit    Gross Profit   Inventory left
-------------------------------------------------------------------------------
               5           25.00         13.75           38.75               95
-------------------------------------------------------------------------------


MENU:
1. DSIPLAY INVENTORY ON HAND
2. PLACE NEW ORDER
3. CLOSE FOR THE DAY
4. QUIT

4
QUITTING...
*/

//TEST 4 (queue gets full)

/*

MENU:
1. DSIPLAY INVENTORY ON HAND
2. PLACE NEW ORDER
3. CLOSE FOR THE DAY
4. QUIT

1
The warehouse currently has 0 widgets in the queue
There can be 2 more orders placed today
100 more widgets can be proccessed today


MENU:
1. DSIPLAY INVENTORY ON HAND
2. PLACE NEW ORDER
3. CLOSE FOR THE DAY
4. QUIT

2
How many widgets would you like to place an order for:
2
Select a Rush Status for your 2 widget order:
1. Standard (33% markup)
2. Expedite (55% markup)
3. Extreme (77% markup)

2


MENU:
1. DSIPLAY INVENTORY ON HAND
2. PLACE NEW ORDER
3. CLOSE FOR THE DAY
4. QUIT

2
How many widgets would you like to place an order for:
3
Select a Rush Status for your 3 widget order:
1. Standard (33% markup)
2. Expedite (55% markup)
3. Extreme (77% markup)

3


MENU:
1. DSIPLAY INVENTORY ON HAND
2. PLACE NEW ORDER
3. CLOSE FOR THE DAY
4. QUIT

2
The warehouse cannot process any more orders for the day.


MENU:
1. DSIPLAY INVENTORY ON HAND
2. PLACE NEW ORDER
3. CLOSE FOR THE DAY
4. QUIT

1
The warehouse currently has 5 widgets in the queue
There can be 0 more orders placed today
95 more widgets can be proccessed today


MENU:
1. DSIPLAY INVENTORY ON HAND
2. PLACE NEW ORDER
3. CLOSE FOR THE DAY
4. QUIT

3

Tracking #    Rush Status    Amount Ordered       % Markup          Price           Cost   Total Markup
-------------------------------------------------------------------------------------------------------
       102        EXTREME                 3          77.00          26.55          15.00          11.55
       101       EXPEDITE                 2          55.00          15.50          10.00           5.50
-------------------------------------------------------------------------------------------------------


DAY 1 SUMMARY:

Number of orders    Cost to Make    Net Profit    Gross Profit   Inventory left
-------------------------------------------------------------------------------
               5           25.00         17.05           42.05               95
-------------------------------------------------------------------------------


MENU:
1. DSIPLAY INVENTORY ON HAND
2. PLACE NEW ORDER
3. CLOSE FOR THE DAY
4. QUIT

1
The warehouse currently has 0 widgets in the queue
There can be 2 more orders placed today
100 more widgets can be proccessed today


MENU:
1. DSIPLAY INVENTORY ON HAND
2. PLACE NEW ORDER
3. CLOSE FOR THE DAY
4. QUIT

4
QUITTING...
*/