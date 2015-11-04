#pragma once


#ifndef MESSAGE_H
#define MESSAGE_H 

# include <string.h>
# include <iostream>
# include <vector> 
# include "Order.h"
# include "Orderbook.h"

using namespace std; 

class Message
{

public:
	Message();
	void receivedBuyOrder(string ticker, string side, string type, double qty, double limit_price, string counterparty);
	void receivedSellOrder(string ticker, string side, string type, double qty, double limit_price, string counterparty);
	void filledOrder(string ticker, double executionQty, double executionPrice, string buyer, string seller);
	void placedInOrderbook(string ticker, string side, string type, double qty, double limit_price, string counterparty);
	void insertToNewMapPrice(Order order); 
	void insertToVector(Order order, int placeInVector); 
	void resetOrderQuantity(Order order);
	void removedFromOrderbook(Order order);
	void emptyOrderbookMarket(); 

private:
	double executionQty; 
	double exectionPrice; 
	string buyer;
	string seller;
 

	// Order information 
	string ticker;
	string side;
	string type;
	double qty;
	double limit_price;
	string counterparty;

};

# endif // Message_H