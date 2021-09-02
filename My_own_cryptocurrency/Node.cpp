// Standard dependencies
#include <iostream>
#include <vector>


// My own dependencies
#include "Node.h"

constexpr auto DB_NAME = "blockchain0";

using namespace My_own_crypto;

Node::Node(const char* wallet_name, const char* version): wallet(Wallet(wallet_name)),version(version), blockchain(DB_operations(DB_NAME)), blockchain_head(0){
	this->blockchain_head = blockchain.get_head();
}

Node::~Node() {
	this->blockchain_head = -5;
}

void Node::run_server(std::string port) {
	std::thread test(&runServer, std::ref(this->blockchain), std::ref(this->blockchain_head), std::ref(this->confirmed_tansactions), port.c_str());
	test.detach();
}

void Node::check_node(std::string port) {
	runClient("127.0.0.1", port, this->blockchain_head, this->received_transactions, this->received_blocks);
	std::cout << "Retrieved transactions: " << this->received_transactions.size() << std::endl;
	std::cout << "Retrieved blocks: " << this->received_blocks.size() << std::endl;
	/*
	for (Transaction i : this->received_transactions) {
		std::cout << i << std::endl;
	}
	for (Block i : received_blocks) {
		std::cout << i << std::endl;
	}*/

}

void Node::create_tx() {

	//std::cout << version << std::endl;
	Transaction tx(Tools::time_now(),this->version, this->wallet.get_compressedPublic());
	std::vector<Entity> unspent_outs;
	std::string destination;
	float balance, total, value;

	balance = 0.f;
	this->blockchain.get_unespent_tx(unspent_outs, tx.origin, Tools::time_now());
	balance = this->blockchain.get_balance(wallet.get_compressedPublic());

	
	std::cout << "Creating transaction with wallet <" << wallet.get_name() << "> on blockchain <" << DB_NAME << ">.\n\n";
	std::cout << "Balance: " << balance << std::endl;
	total = 0.f;

	try {

		do {
			std::cout << "\nAdress: ";
			std::cin >> destination;

			if (destination.size() < 12) {
				throw std::exception("This string will break some fuctions.");
			}

			std::cout << "Quantity: ";
			std::cin >> value;
			value = floor(value * 100) / 100;
			total += value;

			tx.outputs.push_back(Entity(destination, value));

			std::cout << "\nOne more outputs ";
			std::cin.clear();
		} while (Tools::cont_loop());


		std::cout << "\nTransaction fee: ";
		std::cin >> tx.fee;
		total += tx.fee;

		std::cout << "\x1B[2J\x1B[H";
		std::reverse(unspent_outs.begin(), unspent_outs.end());

		if (total <= balance) {
			for (Entity i : unspent_outs) {
				if (total > 0) {
					total -= i.value;
					tx.inputs.push_back(i.account.c_str());
				}
				else {
					break;
				}
			}
			if (total < 0) {
				tx.outputs.push_back(Entity(tx.origin, (total * -1)));
			}
			tx.compute_hash();
			tx.signature = this->wallet.sign_tx(tx.tx_to_json(false));

			std::cout << tx << std::endl;
			if (this->validate_tx(tx))
				this->confirmed_tansactions.push_back(tx);
			else
				std::cout << "There is some wrong information on your transaction, it will not get propagated." << std::endl;
		}
		else {
			std::cout << "Insuficient balance for this transaction.\n\n Operation canceled." << std::endl;
		}

	}catch(std::exception e){

		std::cout << "\x1B[2J\x1B[H\n";
		std::cout << "Transaction creation failed, try again later." << std::endl;
	}

}



const void Node::create_block(Block& block) {}

const float Node::validate_inputs(std::vector<std::string> inputs, std::string address, std::string time) {
	
	std::vector<Entity> inputs_full;
	float sum = 0.f;

	for (std::string i : inputs)
		inputs_full.push_back(Entity(i, 0));

	blockchain.get_inputs(inputs_full, address, time);

	for (Entity i : inputs_full) {
		
		if (i.value <= 0)
			return -1.f;
		sum += i.value;
	}
		
	return sum;
}

const bool Node::validate_sign(Transaction tx) {
	std::string signature = tx.signature;
	tx.signature = "";
	if (!Tools::sign_verifier(tx.origin, signature, tx.tx_to_json()))
		return false;
	return true;
}

const bool Node::validate_tx(Transaction tx) {

	bool valid = true;

	if (tx.version != "1.0.0")
		valid = false;

	// Checking for sign
	if (!validate_sign(tx))
		valid = false;

	float check_inputs = validate_inputs(tx.inputs, tx.origin, tx.time);
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
			if (tx.outputs[i].account == tx.outputs[j].account && i != j)
				return false;
		}
	}

	// Checking time integrity
	if (!Tools::check_date(tx.time))
		valid = false;

	return valid;
}

const bool Node::validate_block(Block block) {
	bool valid = true;

	if (block.version != "1.0.0")
		valid = false;

	//if (block.ID != this->blockchain_head + 1)
		//valid = false;

	if (Tools::hash_sha256(block.block_to_json()) != block.work_hash)
		valid = false;

	for (Transaction i : block.transaction_list) {
		if (!this->validate_tx(i) && i.hash != "477ED9817BCA3D870CF4FB06BA26951CFC865B6B4641D4C85BD8A30F006BFD6D")
			valid = false;
		if (1 != Tools::is_older(i.time, block.time))
			valid = false;
			
	}

	return valid;
}



