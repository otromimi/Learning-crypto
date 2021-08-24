// Standard dependencies
#include <iostream>
#include <vector>


// My own dependencies
#include "Node.h"

constexpr auto DB_NAME = "my_blockchain";

using namespace My_own_crypto;

Node::Node(const char* wallet_name): wallet(Wallet(wallet_name)), blockchain(DB_operations(DB_NAME)), blockchain_head(0){
	this->blockchain_head = blockchain.get_head();
}

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

const float Node::validate_inputs(std::vector<std::string> inputs, std::string address) {
	
	std::vector<Entity> inputs_full;
	float sum = 0.f;

	for (std::string i : inputs)
		inputs_full.push_back(Entity(i, 0));

	blockchain.get_inputs(inputs_full, address);

	for (Entity i : inputs_full) {
		if (i.value <= 0)
			return -1.f;
		sum += i.value;
	}
		
	return sum;
}

const bool Node::validate_tx(Transaction tx) {

	bool valid = true;

	// Checking for sign
	if (!Tools::sign_verifier(tx.origin, tx.signature, tx.tx_to_json()))
		valid = false;

	float check_inputs = validate_inputs(tx.inputs, tx.origin);
	float sum_outputs_and_fee = tx.fee;
	for (Entity i : tx.outputs)
		sum_outputs_and_fee += i.value;

	// Valid outputs, and same amount of value in|out.
	if (check_inputs <= 0 || check_inputs != sum_outputs_and_fee)
		valid = false;

	// Outputs under threshold.
	if (tx.outputs.size() >= MAX_OUT_TX)
		valid = false;
	
	// Only one output per address.
	for (int i = 0; i < tx.outputs.size(); i++) {
		for (int j = i; j < tx.outputs.size(); j++) {
			if (tx.outputs[i].account == tx.outputs[j].account)
				return false;
		}
	}

	if (!Tools::check_date(tx.time))
		valid = false;

	return valid;
}

const void Node::validate_block(const Block block) {}



