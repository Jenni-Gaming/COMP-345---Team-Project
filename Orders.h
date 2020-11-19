#ifndef ORDERS_H
#define ORDERS_H

#include "Cards.h"
#include "Player.h"
#include "Territory.h"
//#include "GameEngine.h"
#include <list>
#include <iostream>
#include <vector>
using namespace std;

class Order {
private:
	bool valid;
	vector<string> vect = { "depl", "adv", "attack", "defend", "draw", "negoc" };
	int typeId;
	string nameOrder;
protected:
	Territory* source;
	Territory* target;

	Player* player1;
	Player* enemy;

public:
	Order();
	Order(string name);
	void validate();
	virtual void execute() = 0; // pure virtual function

	void setTypeId(int n);
	string getType();
	string getResult();

	friend ostream& operator << (ostream& out, const Order& o);
	friend istream& operator >> (istream& in, Order& o);

	~Order(); // Destructor
};

class OrdersList {
public:
	void setOrdersList(Order* order);
	vector<Order*>* getOrdersList();

	void remove(Order* order);
	void move(int initPos, int finalPos);

private:
	vector<Order*> vectOrdersList;
};

// Implemented each kind of order as a subclass of Order class
class Deploy : public Order {
private:
	string deplType = { "deploy" };
public:
	Deploy();
	
	string* getType();
	
	void validate();
	void execute();

	~Deploy();
};

class Advance : public Order {
public:
	Advance();
	
	void validate();
	void execute();

	~Advance();
};

class Bomb : public Order {
public:
	Bomb();
	
	void validate();
	void execute();

	~Bomb();
};

class Blockage : public Order {
public:
	Blockage();
	
	void validate();
	void execute();

	~Blockage();
};

class Airlift : public Order {
public:
	Airlift();
	
	void validate();
	void execute();

	~Airlift();
};

class Negociate : public Order {
public:
	Negociate();

	void validate();
	void execute();

	~Negociate();
};

static string orders[] = { "deploy", "advance", "bomb", "blockage", "airlift", "negociate" };
#endif