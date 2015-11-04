#pragma once
#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include <iostream>
#include <vector> 
#include <map>
#include <functional>

#include "Message.h"
#include "Order.h"

using namespace std;

class Orderbook
{
public:
	Orderbook(vector<Order> allTickerOrders);
	Orderbook(); 
	void buyLimitOrder(Order order); 
	void buyMarketOrder(Order order);
	void sellLimitOrder(Order order);
	void sellMarketOrder(Order order);
	void insertOrdertoBid(Order order); 
	void insertOrdertoAsk(Order order); 
	void orderbookDiagnostics();



private:
	map <double, vector<Order>, greater<double>> Bid; // price and vector of orders at price- highest price first 
	map<double, vector<Order>, less<double>> Ask;  // similar - lowest price first 
};

#endif 