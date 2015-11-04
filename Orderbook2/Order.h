#pragma once

#ifndef ORDER_H
#define ORDER_H

#include <string>

using namespace std; 

class Order
{

public:
	Order(string ticker, string side, string type, double qty, double limit_price, string counterparty);
	string getTicker(); 
	string getSide(); 
	string getType(); 
	double getQty(); 
	double getLimitPrice(); 
	string getCounterParty(); 
	void setQty(double newQty); 


private: 
	string ticker; 
	string side;
	string type;
	double qty;
	double limit_price;
	string counterparty;

};


#endif // ORDER_H