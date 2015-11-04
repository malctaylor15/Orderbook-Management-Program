// Orderbook-Management-Program

This C++ program mimicks a simple exchange that receives orders and finds a matching order using STL maps. 
The program has a file which contains the orders to read in. The orders should have a ticker, side (Buy 'B' or Sell 'S'), 
type (market or limit), number of shares, price, and name. 

As in exchanges, if an a participant is trying to buy shares of a ticker, they are looking for the lowest price. 
If a participant is looking to sell, they want the highest price. 
Limit orders get saved for later dates if there is not an existing order at the price. 
Market orders look for the quantity of shares, disregarding the price, and unfilled requests are not saved. 

The program also messages the participants when the orders are modified. 

