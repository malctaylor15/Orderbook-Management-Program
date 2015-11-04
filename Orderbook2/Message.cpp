#include "Message.h"
#include "Orderbook.h"
#include <vector> 
#include <map> 
#include "Order.h"
#include <utility>
#include <string>
#include <iostream>

Message::Message()
{

}

void Message::receivedBuyOrder(string ticker, string side, string type, double qty, double limit_price, string counterparty)
{
	cout << counterparty << ": your order has been received to buy: " << qty << " shares of " << ticker << " at $" << limit_price << endl;

}

void Message::receivedSellOrder(string ticker, string side, string type, double qty, double limit_price, string counterparty)
{
	cout << counterparty << ": your order has been received to sell: " << qty << " shares of " << ticker << " at $" << limit_price << endl;

}

void Message::filledOrder(string ticker, double executionQty, double executionPrice, string buyer, string seller)
{
	cout << buyer << ": your order has been completed to buy " << executionQty << " shares of " << ticker << " at $" << executionPrice << endl;
	cout << seller << ": your order has been completed to sell " << executionQty << " shares of " << ticker << " at $" << executionPrice << endl;
}

void Message::placedInOrderbook(string ticker, string side, string type, double qty, double limit_price, string counterparty)
{
	if (side == "B") 
	{
		cout << counterparty << ": your order to buy was incomplete. You have " << qty << " shares remaining in the orderbook at $" << limit_price << endl; 
	}
	else if (side == "S")
	{
		cout << counterparty << ": your order to sell was incomplete. You have " << qty << " shares remaining in the orderbook at $" << limit_price << endl;

	}
}

void Message::insertToNewMapPrice(Order order)
{
	cout << order.getCounterParty() << " : Your order was inserted in the orderbook. " << endl; 
	cout << "You have " << order.getQty() << " shares at $" << order.getLimitPrice() << " remaining in the orderbook." << endl; 
	cout << "You are the only order at this price. " << endl; 
}

void Message::insertToVector(Order order, int placeInVector)
{

	cout << order.getCounterParty() << ": Your order was inserted in the orderbook. " << endl;
	cout << "You have " << order.getQty() << " shares at $" << order.getLimitPrice() << " remaining in the orderbook." << endl;
	cout << "You are the "<<placeInVector<< " order at this price." << endl;
}

void Message::resetOrderQuantity(Order order)
{
	cout << order.getCounterParty() << ": You have " << order.getQty() << " shares  in the orderbook at $"<<order.getLimitPrice() << endl; 
}

void Message::removedFromOrderbook(Order order)
{

	cout << order.getCounterParty() << ": Your order has been fulfilled of "<< order.getQty() << " shares at $" << order.getLimitPrice()<<" and removed from the orderbook." << endl; 
}

void Message::emptyOrderbookMarket()
{
	cout << "There are no orders in the orderbook. Your order was not fulfilled. " << endl; 
}

