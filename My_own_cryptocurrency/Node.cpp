// Standard dependencies
#include <iostream>


// My own dependencies
#include "Node.h"

using namespace My_own_crypto;

Node::Node(const char* wallet_name): wallet(Wallet(wallet_name)){}

const Transaction Node::create_tx() {

	//std::cout << version << std::endl;

	std::string destination;
	unsigned int value;
	std::string origin("1330662819286567003101256740359821157367793328918");
	unsigned int fee;

	std::cout << "Beneficiary: ";
	getline(std::cin, origin);

	std::cout << "Value: ";
	std::string value_s;
	getline(std::cin, value_s);
	value = std::stoi(value_s);

	std::cout << "Fee: ";
	std::string fee_s;
	getline(std::cin, fee_s);
	fee = std::stoi(fee_s);


	Transaction tx(origin, value, fee);

	// Serialization of the transaction structure in a JSON string
	//const std::string serialize_tx = Transaction::tx_to_json(tx);
	//std::cout << tx << std::endl;


	//tx.tx_id = Node::hash_digest(serialize_tx);

	//std::cout << tx << std::endl;
	return tx;

}

const void Node::create_block(Block& block) {}

const void Node::validate_inputs(std::string input) {}

const void Node::validate_tx(const Transaction tx) {}

const void Node::validate_block(const Block block) {}



