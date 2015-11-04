// Malcolm Taylor and Karen Pardo 
#include <vector> 
#include <map> 
#include <utility>
#include <string>
#include <iostream>

#include "Order.h"
#include "Orderbook.h"
#include "Message.h"

using namespace std;


Orderbook::Orderbook()
{
	 
}


void Orderbook::insertOrdertoBid(Order order)
{
	Message *insertMessage = new Message(); 
	// look for order price set (key & vector) in the bid map
	map<double, vector<Order>>::iterator orderInMap = Bid.find(order.getLimitPrice());
	if (orderInMap == Bid.end()) // No existing price in map
	{
		vector<Order> newVector;
		newVector.push_back(order);
		Bid.insert(pair <double, vector<Order>>(order.getLimitPrice(), newVector));
		insertMessage->insertToNewMapPrice(order); 
	}
	else // Map already has orders with same price (put order in vector of orders (of the same price)) 
	{
		orderInMap->second.push_back(order);
		insertMessage->insertToVector(order, (int)orderInMap->second.size()); 
	}
	
	// Garbage removal 
	delete insertMessage;
}

void Orderbook::insertOrdertoAsk(Order order)
{
	Message *insertMessage = new Message(); 
	// look for order price set (key & vector) in the ask map
	map<double, vector<Order>>::iterator orderInMap = Ask.find(order.getLimitPrice());
	if (orderInMap == Ask.end()) // had not found 
	{
		vector<Order> newVector;
		newVector.push_back(order);
		Ask.insert(pair <double, vector<Order>>(order.getLimitPrice(), newVector));
		insertMessage->insertToNewMapPrice(order);
	}

	else // Map already has orders with same price 
	{
		orderInMap->second.push_back(order);
		insertMessage->insertToVector(order, (int)orderInMap->second.size());
	}

	delete insertMessage; 
}

void Orderbook::buyLimitOrder(Order order)
{
	Message *buyMessage = new Message();

	// Check if order book is empty
	bool OrderbookEmpty = Ask.empty();
	if (OrderbookEmpty)
	{
		insertOrdertoBid(order);
	}
	else { // Attempt to fill order 

		map<double, vector<Order>> ::iterator bestAskMapLine = Ask.begin(); // refers to vector of best prices 
		vector<Order> ::iterator bestAskOrder = bestAskMapLine->second.begin(); // refers to first order in vector of best prices 

		if (order.getLimitPrice() >= bestAskMapLine->first) // ?order price < first key price - order will be at least partially filled 
		{
			if (order.getQty() > bestAskOrder->getQty()) // incoming order is partially filled 
			{
			
				// Decrease incoming order quantity and set new Qty for incoming order  
				double newQty = order.getQty() - bestAskOrder->getQty();
				order.setQty(newQty);
				buyMessage->resetOrderQuantity(order); 


				// Remove order from order book 
				buyMessage->removedFromOrderbook(*bestAskOrder);
				bestAskMapLine->second.erase(bestAskOrder);
				
				// Remove price in Map if no other orders at the price 
				if (bestAskMapLine->second.empty())
				{
					Ask.erase(bestAskMapLine);
				}

				// Re call function to attempt to fill order again (with new quantity) 
				buyLimitOrder(order);
			}
			else // order completely filled 
			{
				double newQty = bestAskOrder->getQty() - order.getQty();

				if (newQty == 0) // Map qty = order qty 
				{

					// Remove order from order book 
					buyMessage->removedFromOrderbook(*bestAskOrder);
					bestAskMapLine->second.erase(bestAskOrder);

					// Delete empty keys for maps if no orders are in the vector 
					if (bestAskMapLine->second.empty())
					{
						Ask.erase(bestAskMapLine);
					}
				}
				else
				{
					// Reduce quantity of order in the order book 
					bestAskOrder->setQty(newQty);
					buyMessage->resetOrderQuantity(*bestAskOrder); 
				}
			}
		}
		else // put order in orderbook  
		{
			insertOrdertoBid(order);
		}
	}
}

void Orderbook::buyMarketOrder(Order order) // Very similar to BuyLimitOrder, does not check price 
{
	Message *buyMessage = new Message();

	// Check if order book is empty
	bool OrderbookEmpty = Ask.empty();
	if (OrderbookEmpty)  // the order book is empty- the market order will not be filled
	{
		buyMessage->emptyOrderbookMarket(); 
	}
	else // attempt to fill the order 
	{

		map<double, vector<Order>> ::iterator bestAskMapLine = Ask.begin();
		vector<Order> ::iterator bestAskOrder = bestAskMapLine->second.begin();

		if (order.getQty() > bestAskOrder->getQty()) // New order will fill order from orderbook  
		{
			// Decrease order quantity and set new Qty  
			double newQty = order.getQty() - bestAskOrder->getQty();
			order.setQty(newQty);
			buyMessage->resetOrderQuantity(order); 

			// Remove order from order book 
			buyMessage->removedFromOrderbook(*bestAskOrder);
			bestAskMapLine->second.erase(bestAskOrder);
			
			// Delete empty keys for maps if no orders are in the vector 
			if (bestAskMapLine->second.empty())
			{
				Ask.erase(bestAskMapLine);
			}

			// Re call function to attempt to fill order again (with new quantity) 
			buyMarketOrder(order);
		}
		else // order completely filled, new order qty > orderbook qty  
		{
			// Remaining qty to be filled 
			double newQty = bestAskOrder->getQty() - order.getQty();

			if (newQty == 0) // Map qty = order qty 
			{
				// Remove order from orderbook 	
				buyMessage->removedFromOrderbook(*bestAskOrder);
				bestAskMapLine->second.erase(bestAskOrder);

				// Delete empty keys for maps if no orders are in the vector 
				if (bestAskMapLine->second.empty())
				{
					Ask.erase(bestAskMapLine);
				}
			}
			else
			{
				// Change the orderbook order quantity 
				bestAskOrder->setQty(newQty);
				buyMessage->resetOrderQuantity(*bestAskOrder); 
			}
		}
	}

	// Garbage collection 
	delete buyMessage;
}

void Orderbook::sellLimitOrder(Order order)
{
	Message *sellMessage = new Message();

	bool OrderbookEmpty = Bid.empty(); 	// Check if order book is empty
	if (OrderbookEmpty) // if the order book is empty, insert the order 
	{
		insertOrdertoAsk(order);
	}
	else
	{
		map<double, vector<Order>> ::iterator bestBidMapLine = Bid.begin();
		vector<Order> ::iterator bestBidOrder = bestBidMapLine->second.begin();

		if (order.getLimitPrice() <= bestBidMapLine->first) // check if order price < first key price 
		{
			if (order.getQty() > bestBidOrder->getQty()) // order is partially filled 
			{
				// Decrease order quantity and set new Qty  
				double newQty = order.getQty() - bestBidOrder->getQty();
				order.setQty(newQty);
				sellMessage->resetOrderQuantity(order);

				// Remove order from order book 
				sellMessage->removedFromOrderbook(*bestBidOrder);
				bestBidMapLine->second.erase(bestBidOrder);

				// Delete empty keys for maps if no orders are in the vector 
				if (bestBidMapLine->second.empty())
				{
					Bid.erase(bestBidMapLine);
				}

				// Re call function to attempt to fill order again (with new quantity) 
				sellLimitOrder(order);

			}
			else if (order.getLimitPrice() < bestBidMapLine->first) // order completely filled 
			{
				double newQty = bestBidOrder->getQty() - order.getQty();

				if (newQty == 0) // Map qty = order qty - orders will be filled and orderbook order removed  
				{
					// Remove order from orderbook
					sellMessage->removedFromOrderbook(*bestBidOrder);
					bestBidMapLine->second.erase(bestBidOrder);

					// Delete empty keys for maps if no orders are in the vector 
					if (bestBidMapLine->second.empty())
					{
						Bid.erase(bestBidMapLine);
					}
				}

				else // if newQty > 0 
				{
					// Reset orderbook order quantity 
					bestBidOrder->setQty(newQty);
					sellMessage->removedFromOrderbook(*bestBidOrder);

				}
			}
		} 
			else // put order in orderbook  
			{
				insertOrdertoAsk(order);
			}

		}
		// Garbage collection 
		delete sellMessage;
	}


void Orderbook::sellMarketOrder(Order order)
{
	Message *sellMessage = new Message();
	bool OrderbookEmpty = Ask.empty();
	if (OrderbookEmpty)  // The orderbook is empty so the market order will not be filled 
	{
		sellMessage->emptyOrderbookMarket(); 
	}

	else 
	{
		map<double, vector<Order>> ::iterator bestBidMapLine = Bid.begin();
		vector<Order> ::iterator bestBidOrder = bestBidMapLine->second.begin();

		if (order.getQty() > bestBidOrder->getQty()) // New order will fill order from orderbook  
		{
			// Decrease order quantity and set new Qty  
			double newQty = order.getQty() - bestBidOrder->getQty();
			order.setQty(newQty);
			sellMessage->resetOrderQuantity(order); 

			// Remove order from order book 
			sellMessage->removedFromOrderbook(*bestBidOrder);
			bestBidMapLine->second.erase(bestBidOrder);

			// Delete empty keys for maps if no orders are in the vector 
			if (bestBidMapLine->second.empty())
			{
				Bid.erase(bestBidMapLine);
			}

			// Re call function to attempt to fill order again (with new quantity) 
			sellMarketOrder(order);
		}
		else // order completely filled 
		{
			double newQty = bestBidOrder->getQty() - order.getQty();

			if (newQty == 0) // Map qty = order qty 
			{
				// Remove order from orderbook
				sellMessage->removedFromOrderbook(*bestBidOrder);
				bestBidMapLine->second.erase(bestBidOrder);

				// Delete empty keys for maps if no orders are in the vector 
				if (bestBidMapLine->second.empty())
				{
					Bid.erase(bestBidMapLine);
				}
			}
			else
			{
				// Change the orderbook order quantity 
				bestBidOrder->setQty(newQty);
				sellMessage->resetOrderQuantity(*bestBidOrder); 
			}
		}

	}
	// Garbage Collection 
	delete sellMessage;
}

void Orderbook::orderbookDiagnostics()
{
	map <double, vector<Order>, less<double>>::iterator Bid_iterator = Bid.begin();
	map<double, vector<Order>, greater<double>> ::iterator Ask_iterator = Ask.begin(); 
	
	cout << endl<< "Ask Side!" << endl;
	for (Ask_iterator; Ask_iterator != Ask.end(); Ask_iterator++)
	{
		cout << "Price: " << Ask_iterator->first << endl;
		for (vector<Order>::iterator VectPrices = Ask_iterator->second.begin(); VectPrices != Ask_iterator->second.end(); VectPrices++)
		{
			cout << "CP:" << VectPrices->getCounterParty() <<"\t Qty:"<<VectPrices->getQty()<< endl;
		}
	}

	cout << endl<< "Bid Side!" << endl;
	for (Bid_iterator; Bid_iterator != Bid.end(); Bid_iterator++)
	{
		cout << "Price: " << Bid_iterator->first<<endl;
		for (vector<Order>::iterator VectPrices = Bid_iterator->second.begin(); VectPrices != Bid_iterator->second.end(); VectPrices++)
		{
			cout << "CP:" << VectPrices->getCounterParty() <<"\t Qty: " <<VectPrices->getQty()<< endl;
		}
	}

}


