// Standard dependencies
#include <iostream>


// My own dependencies
#include "Node.h"

using namespace My_own_crypto;

Node::Node(const char* wallet_name): wallet(Wallet(wallet_name)){}

const Transaction Node::create_tx() {

	//std::cout << version << std::endl;
	Transaction tx(Tools::time_now(), this->wallet.get_compressedPublic());
	std::string destination;
	float value;
	
	std::cout << "Creating transaction with wallet <" << wallet.get_name() << ">.\n\n";

	do{
		std::cout << "\nAdress: ";
		std::cin >> destination;
		std::cout << "Quantity: ";
		std::cin >> value;
		tx.outputs.push_back(Entity(destination, value));

		std::cout << "\nAdd another out ";
	}while (Tools::cont_loop());


	std::cout << "\nFee for the transaction: ";
	std::cin >> tx.fee;


	return tx;

}



const void Node::create_block(Block& block) {}

const void Node::validate_inputs(std::string input) {}

const void Node::validate_tx(const Transaction tx) {}

const void Node::validate_block(const Block block) {}



