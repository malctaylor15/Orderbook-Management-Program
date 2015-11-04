#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <map>

#include "Message.h"
#include "Orderbook.h"
#include "Order.h"

using namespace std; 

int main()
{
	// Open custom file 
	ifstream infile;
	infile.open("Order.txt");

	// declare variables for storing into Bond
	string ticker;
	string side;
	string type;
	double qty;
	double limit_price;
	string counterparty;
	map<string, Orderbook> orderbookMap;
	string pause;  

	infile.ignore(30, '\n');


	while (infile.good())
	{
		// Read data from file and store into order 
		infile >> ticker >> side >> type >> qty >> limit_price >> counterparty;
		Order *order = new 	Order(ticker, side, type, qty, limit_price, counterparty);

		cout << endl << "Read in order from file with details:" << endl;
		cout << "Ticker: " << ticker << "     Side: " << side << "      Type: " << type << endl;
		cout << "Qty: " << qty << "        Price: " << limit_price << "     CP: " << counterparty << endl << endl;

		// Make new Orderbook for each ticker or find orderbook with ticker 
		map<string, Orderbook>::iterator currentOrderbookTickerFinder = orderbookMap.find(order->getTicker());
		if (currentOrderbookTickerFinder == orderbookMap.end()) // no orderbook for ticker (make new orderbook)
		{
			cout << "New Orderbook was made with ticker " << ticker << endl; 
			Orderbook *newOrderbook = new Orderbook();
			orderbookMap.insert(std::pair <string, Orderbook>(order->getTicker(), *newOrderbook));
		}

		// find the orderbook with the correct ticker 
		map<string, Orderbook>::iterator currentOrderbookTicker = orderbookMap.find(order->getTicker());



		Message* newMessage = new Message();
		if (side == "B")
		{
			newMessage->receivedBuyOrder(order->getTicker(), order->getSide(), order->getType(), order->getQty(), order->getLimitPrice(), order->getCounterParty());
			delete newMessage;

			if (type == "market")
			{
				currentOrderbookTicker->second.buyMarketOrder(*order);
			}
			else if (type == "limit")
			{
				currentOrderbookTicker->second.buyLimitOrder(*order);
			}
		}

		else if (side == "S")
		{

			newMessage->receivedSellOrder(ticker, side, type, qty, limit_price, counterparty);
			delete newMessage;

			if (type == "market")
			{
				currentOrderbookTicker->second.sellMarketOrder(*order);
			}
			else if (type == "limit")
			{
				currentOrderbookTicker->second.sellLimitOrder(*order);
			}

		}
		cout << endl << "Press y for information about Orderbook or any key to continue :  ";   
		cin >> pause;
		if (pause == "y")
		{
			currentOrderbookTicker->second.orderbookDiagnostics();
		}
	}

	infile.close();
	cout << "End reading orders" << endl; 
}

