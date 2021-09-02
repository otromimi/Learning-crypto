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

const void Node::print_block() {

	this->blockchain_head = this->blockchain.get_head();

	std::cout << "\x1B[2J\x1B[H\n";

	unsigned int blk_id;
	Block blk;

	std::cout << "Block id: ";
	std::cin >> blk_id;

	if (blk_id <= this->blockchain_head && blk_id > 0) {
		this->blockchain.get_block(blk, blk_id);
		std::cout <<  "\x1B[2J\x1B[H\n" << blk << std::endl;
	}
	else {
		std::cout <<  "\x1B[2J\x1B[H\n" << " The block with ID " << blk_id << " wasn't found\n\n  Max blockchain depth: " << this->blockchain_head << "\n" << std::endl;
	}

}


const void Node::print_transaction() {

	std::string tx_hash;
	std::vector<std::string> tx_list;
	Transaction tx;

	do {

		std::cout << "\x1B[2J\x1B[H\n" << "Transaction hash: ";
		std::cin >> tx_hash;
		tx_list.clear();
		std::cout << "\x1B[2J\x1B[H\n";

		this->blockchain.get_tx_count(tx_list, tx_hash);
		if (tx_list.size() > 1) {
			std::cout << "The following transactions were found: \n";
			for (std::string i : tx_list)
				std::cout << "\t" << i << "\n";
			std::cout << std::endl;
		}
		else if (tx_list.size() == 1) {
			this->blockchain.get_tx(tx, tx_hash);
			std::cout << tx << std::endl;
		}
		else {
			std::cout << "No transactions were found with hash: " << (tx_hash.size() < 64 ? "..." + tx_hash : tx_hash) << "\n" << std::endl;
		}
		std::cout << "Search for another transaction";
	} while (Tools::cont_loop());
}



const void Node::print_wallet_info() {

	std::string pk = this->wallet.get_compressedPublic();
	float balance = this->blockchain.get_balance(pk);
	std::vector<Entity> unspent_outs;
	int under1 = 0, under10 = 0, under100 = 0, under1000 = 0, over1000 = 0;

	this->blockchain.get_unespent_tx(unspent_outs, pk, Tools::time_now());

	for (Entity i : unspent_outs) {
		if (i.value < 1.f)
			under1++;
		else if (i.value < 10.f)
			under10++;
		else if (i.value < 100.f)
			under100++;
		else if (i.value < 1000.f)
			under1000++;
		else if (i.value > 1000.f)
			over1000++;
	}

	std::cout << "\x1B[2J\x1B[H" << 
		"\n User: " << this->wallet.get_name() <<
		"\n Blockchain: " << DB_NAME <<
		"\n Account address: " << pk <<
		"\n Balance: " << balance <<
		"\n Wallet fragmentation{ " << 
		"\n\t" << under1 <<" utx under 1" <<
		"\n\t" << under10 << " utx under 10" <<
		"\n\t" << under100 << " utx under 100" <<
		"\n\t" << under1000 << " utx under 1000" <<
		"\n\t" << over1000 << " utx over 1000" <<
		"\n }" <<
		"\n" << std::endl;

}