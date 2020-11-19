#include "Orders.h"
#include <list>
#include<iostream>
#include <vector>
using std::cout;

Order::Order() {
	nameOrder = "";
}

Order::Order(string name) {
	this->nameOrder = name;
}

void Order::validate(){ // will eventually verify if an order is valid
	cout << "In order::validate()" << std::endl;
	valid = true;
}

void Order::setTypeId(int n) {
	typeId = n;
}

string Order::getType() {
	return vect.at(typeId);
}

Order::~Order(){
	nameOrder.clear();
}

//ostream& operator<<(ostream& out, const Order& o){
//	out << "order class";
//	return out;
//}
//
//istream& operator>>(istream& in, Order& o){
//	cout << "order";
//	in >> o;
//	return in;
//}

void OrdersList::setOrdersList(Order* order) {
	vectOrdersList.push_back(order);
}

vector<Order*>* OrdersList::getOrdersList() {
	return &vectOrdersList;
}
void OrdersList::remove(Order* order){
	for (int i = 0; i < vectOrdersList.size(); i++) {
		if(order->getType()==vectOrdersList.at(i)->getType()){
			cout << "Deleting order: " << order->getType() << endl;
			vectOrdersList.erase(vectOrdersList.begin() + i);

			return;
		}
	}
}

void OrdersList::move(int initPos, int finalPos){
	if (initPos >= 0 && initPos < vectOrdersList.size() && finalPos >= 0 && finalPos < vectOrdersList.size()) {
		vectOrdersList.insert(vectOrdersList.begin() + finalPos, vectOrdersList.at(initPos));
		vectOrdersList.erase(vectOrdersList.begin() + initPos);
	}
	
	else if (finalPos == vectOrdersList.size()) {
		vectOrdersList.push_back(vectOrdersList.at(initPos));
		vectOrdersList.erase(vectOrdersList.begin() + initPos);
	}
	
	else {
		cout << "Not a valid position" << endl;
	}
}

//Deploy::Deploy(int playerId, int sourceId, int targetId, int armies) {
//	source->setTerritoryId(sourceId);
//	target->setTerritoryId(targetId);
//}

Deploy::Deploy(){
	cout << "Creating Deploy order " << endl;
	setTypeId(0);
}

void Deploy::validate(){
	
}

// A deploy order tells a certain number of armies taken from the 
// reinforcement pool to deploy to a target territory owned by 
// the player issuing this order.
void Deploy::execute(){
	
	string targetName;
	cout << "What is the name of the territory that you want to deploy your army to?" << endl;
	cin >> targetName;

	target->setTerritoryName(targetName);

	int targetOwner=target->getPlayerId();

	int player1Id = this->player1->getPlayerID();

	// If the target territory does not belong to the player that issued
	// the order, the order is invalid
	if (player1Id != targetOwner) {
		cout << "Invalid deploye order" << endl;
	}

	// If the target territory belongs to the player that issued the 
	// order, the selected number of armies is added to the number of 
	// armies in that territory
	if (player1Id == targetOwner) {
		int nbArmies;
		cout << "What is the number of armies you want to add to the territory?" << endl;
		cin >> nbArmies;

		int targetArmy = target->getArmy();
		target->setArmy(targetArmy + nbArmies);

		int sourceArmy=source->getArmy();
		this->source->setArmy(sourceArmy-nbArmies);
	}
}

Deploy::~Deploy(){
	delete target;
	delete source;
	delete player1;
	delete enemy;
}

string* Deploy::getType() {
	return &deplType;
}
Advance::Advance(){
	cout << "Creating Advance order " << endl;
	setTypeId(1);
}

void Advance::validate(){
	cout << "Executing advance::validate()" << std::endl;
}

// An advance order tells a certain number of army units to move from a 
// source territory to a target territory.
void Advance::execute(){

	string targetName;
	cout << "What is the name of the territory that you want to advance your army to?" << endl;
	cin >> targetName;

	target->setTerritoryName(targetName);

	int targetOwner = target->getPlayerId();

	int player1Id = this->player1->getPlayerID();

	int sourceOwner = this->source->getPlayerId();
	// If the source territory does not belong to the player that issued
	// the order, the order is invalid
	if (sourceOwner != player1Id) {

	}

	// If the source and target territory both belong to the player 
	// that issued the order, the army units are moved from source to 
	// target territory
	if (sourceOwner == player1Id && player1Id == targetOwner) {
		int units;
		cout << "What is the number of army units you want to move to you other territory?" << endl;
		cin >> units;
		int sourceArmy = this->source->getArmy();
		source->setArmy(sourceArmy - units);

		int targetArmy = target->getArmy();
		target->setArmy(targetArmy + units);
	}

	// If target territory belongs to another player, an attack is 
	// simulated when the order is executed. An attack is simulated by
	// the following battle simulation mechanism:
	if (sourceOwner == player1Id && player1Id != targetOwner) {
		int attackingArmy = source->getArmy();
		int defendingArmy = target->getArmy();

		// Each attacking army unit involved has 60% chances of 
		// killing one defending army. At the same time, each defending
		// army unit has 70% chances of killing one attacking army unit.
		int remainingAttack;
		int remainingDefend;

		// If all defender's armies are elminiated, the attacker captures
		// the territory. The attacking army units that survived the battle
		// then occupy the conquered territory
		if (remainingDefend == 0 && remainingAttack > 0) {

			enemy->removeTerritoryFromPlayer(target);
			player1->addTerritoryToPlayer(target);
			target->setArmy(remainingAttack);
		}
		// Players receive a card at the end of his turn if they 
		// successfully conquered at least one territory
		// during their turn.
	}
}

Advance::~Advance(){
	delete target;
	delete source;
	delete player1;
	delete enemy;
}

Bomb::Bomb(){
	cout << "Creating Bomb order " << endl;
	setTypeId(2);
}

void Bomb::validate(){
	cout << "Executing bomb::validate()" << std::endl;
}

// A bomb order targets a territory owned by another player than the 
// one issuing the order. Its result is to remove half of the armies from
// this territory. The bomb order can only be created by playing the bomb
// card.
void Bomb::execute(){
	string targetName;
	cout << "What is the name of the territory that you want to bomb?" << endl;
	cin >> targetName;

	target->setTerritoryName(targetName);
	int targetOwner = target->getPlayerId();

	int player1Id = this->player1->getPlayerID();

	//int sourceOwner = this->source->getPlayerId();

	//// If the target belongs to the player that issued the order, 
	//// the order is invalid
	if (targetOwner == player1Id) {
		cout << "Invalid bomb order" << endl;
	}

	// If the target belongs to an enemy player, half the armies are 
	// removed from this territory.
	if (targetOwner != player1Id) {
		int targetArmy = target->getArmy();

		int remainingArmies = targetArmy / 2;

		target->setArmy(remainingArmies);
	}	
}

Bomb::~Bomb(){
	delete target;
	delete source;
	delete player1;
	delete enemy;
}

Blockage::Blockage(){
	cout << "Creating Blockage order " << endl;
	setTypeId(3);
}

void Blockage::validate() {
	cout << "Executing blockage::validate()" << std::endl;
}

// A blockage order targets a territory that belongs to the player 
// issuing the order. Its effect is to doubling the number of armies
// on the territory and transfering the ownership of the territory
// to the Neutral player
void Blockage::execute() {
	string targetName;
	cout << "What is the name of the territory that you want to do a blockage?" << endl;
	cin >> targetName;

	target->setTerritoryName(targetName);
	int targetOwner = target->getPlayerId();

	int player1Id = this->player1->getPlayerID();

	// If the target territory belongs to an enemy player, the order is 
	// declared invalid. Ther blockage order can only be created by
	// playing the blockage card.
	if (targetOwner != player1Id) {
		cout << "Invalid blockage order" << endl;
	}

	// If the target territory belongs to the player issuing the order, 
	// the nummber of armies on the territory is doubled and the ownership
	// of the territory is transferred to the Neutral player.
	if (targetOwner == player1Id) {
		int player1Armies = target->getArmy();
		player1Armies *= 2;

		target->setArmy(player1Armies);
		
		player1->removeTerritoryFromPlayer(target);
		Player* neutral;
		neutral->addTerritoryToPlayer(target);
	}
	
}

Blockage::~Blockage(){
	delete target;
	delete source;
	delete player1;
	delete enemy;
}

Airlift::Airlift(){
	cout << "Creating Airlift order " << endl;
	setTypeId(4);
}

void Airlift::validate(){
	cout << "Executing airlift::validate()" << std::endl;
}

// An airlift order tells a certain number of armies taken from a source 
// territory to be moved to a target territory, the source territory being 
// owned by the player issuing the order. The airlif order can only be 
// created by playing the airlift card.
void Airlift::execute(){
	string targetName;
	cout << "What is the name of the territory that you want to airlift?" << endl;
	cin >> targetName;

	target->setTerritoryName(targetName);
	int targetOwner = target->getPlayerId();

	int player1Id = this->player1->getPlayerID();
	
	int sourceOwner = this->source->getPlayerId();

	
	// If the source or target does not belong to the player that issued 
	// the order, the order is invalid.
	if (sourceOwner != player1Id && player1Id != targetOwner) {

	}

	// If the target territory belongs to the player that issued the 
	// deploy order, the selected number of armies is attacking that
	// terriroty (see "advance order")
	if (sourceOwner = targetOwner && sourceOwner == player1Id) {
		
		// If the source territory does not belong to the player that issued
		// the order, the order is invalid
		if (sourceOwner != player1Id) {

		}

		// If the source and target territory both belong to the player 
		// that issued the order, the army units are moved from source to 
		// target territory
		if (sourceOwner == player1Id && player1Id == targetOwner) {
			int units;
			cout << "What is the number of army units you want to move to you other territory?" << endl;
			cin >> units;
			int sourceArmy = this->source->getArmy();
			source->setArmy(sourceArmy - units);

			int targetArmy = target->getArmy();
			target->setArmy(targetArmy + units);
		}

		// If target territory belongs to another player, an attack is 
		// simulated when the order is executed. An attack is simulated by
		// the following battle simulation mechanism:
		if (sourceOwner == player1Id && player1Id != targetOwner) {
			int attackingArmy = source->getArmy();
			int defendingArmy = target->getArmy();

			// Each attacking army unit involved has 60% chances of 
			// killing one defending army. At the same time, each defending
			// army unit has 70% chances of killing one attacking army unit.
			int remainingAttack;
			int remainingDefend;

			// If all defender's armies are elminiated, the attacker captures
			// the territory. The attacking army units that survived the battle
			// then occupy the conquered territory
			if (remainingDefend == 0 && remainingAttack > 0) {

				enemy->removeTerritoryFromPlayer(target);
				player1->addTerritoryToPlayer(target);
				target->setArmy(remainingAttack);
			}
			// Players receive a card at the end of his turn if they 
			// successfully conquered at least one territory
			// during their turn.
		}
	}
}

Airlift::~Airlift(){
	delete target;
	delete source;
	delete player1;
	delete enemy;
}

Negociate::Negociate(){
	cout << "Creating Negociate order " << endl;
	setTypeId(5);
}

void Negociate::validate(){
	cout << "Executing negociate::validate()" << std::endl;
}

// A negotiate order targets an enemy player. It results in the
// target player and the player issuing the order to not be able 
// to successfully attack each others’ territories for the remainder 
// of the turn. The negotiate order can only be created by playing the 
// diplomacy card
void Negociate::execute(){
	
	int sourceOwner = this->source->getPlayerId();

	int player1Id = this->player1->getPlayerID();

	string targetName;
	cout << "What is the name of the Territory with whom you want to negociate?" << endl;
	cin >> targetName;

	target->setTerritoryName(targetName);
	int targetOwner = target->getPlayerId();
	
	// If the target is the player issuing the order, then the order is 
	// invalid.
	if (targetOwner == player1Id) {
		cout << "Invalid negociate order" << endl;
	}

	// If the target is an enemy player, then the effect is that any 
	// attack that may be declared between territories of the player
	// issuing the negociate order and the target player will result
	// in an invalid order.
	if (targetOwner != player1Id) {
		
	}
}

Negociate::~Negociate(){
	delete target;
	delete source;
	delete player1;
	delete enemy;
}

