#include "Order.h"
#include <iostream>


Order::Order(string ticker, string side, string type, double qty, double limit_price, string counterparty)
{
	this->ticker = ticker; 
	this->side = side;
	this->type = type; 
	this->qty = qty; 
	this->limit_price = limit_price; 
	this->counterparty = counterparty; 
}

string Order::getTicker()
{
	return this->ticker;
}

string Order::getSide()
{
	return side;
}

string Order::getType()
{
	return type; 
}

double Order::getLimitPrice()
{
	return limit_price;
}

double Order::getQty()
{
	return qty;
}

string Order::getCounterParty()
{
	return counterparty;
}

void Order::setQty(double newQty)
{
	qty = newQty;
}
