// Standard dependencies
#include <iostream>
#include <vector>


// My own dependencies
#include "Node.h"

#define TTL 240

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
	

	std::cout << "\x1B[2J\x1B[H\n\n";
	std::cout << "Retrieved transactions: " << this->received_transactions.size() << std::endl;
	std::cout << "Retrieved blocks: " << this->received_blocks.size() << std::endl;
	
	for (int i = 0; i < this->received_transactions.size(); i++) {
		if (this->validate_tx(this->received_transactions[i])) {
			this->received_transactions[i].compute_hash();
			confirmed_tansactions.push_back(this->received_transactions[i]);
		}
	}
	for (Block i : received_blocks) {
		std::cout << i << std::endl;
	}

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
				throw std::exception();
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
			
			tx.signature = this->wallet.sign_tx(tx.tx_to_json(false));
			tx.compute_hash();

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



const void Node::create_block() {

	Block block;
	Block father_blk;
	Transaction aux;

	this->blockchain_head = this->blockchain.get_head();

	// Cleaning old transactions
	for (int i = 0; i < this->confirmed_tansactions.size(); i++) {
		if (Tools::avg_tpu(this->confirmed_tansactions[i].time, Tools::time_now()) > TTL)
			this->confirmed_tansactions.erase(this->confirmed_tansactions.begin()+i);
		if (this->confirmed_tansactions[i].fee <+ 0)
			this->confirmed_tansactions.erase(this->confirmed_tansactions.begin() + i);
	}

	if (this->confirmed_tansactions.size() == 0) {
		std::cout << " There are no valid transactions avaliable right now." << std::endl;
		return;
	}

	// Getting more profitable transactions
	for (int i = 0; i < this->confirmed_tansactions.size(); i++) {
		for (int j = i; j < this->confirmed_tansactions.size(); j++) {
			if (this->confirmed_tansactions[i].fee > this->confirmed_tansactions[j].fee) {
				aux = this->confirmed_tansactions[i];
				this->confirmed_tansactions[i] = this->confirmed_tansactions[j];
				this->confirmed_tansactions[j] = aux;
			}
		}
	}

	std::cout << "Valid transactions pool: \n\n";
	for (Transaction i : this->confirmed_tansactions) {
		i.compute_hash();
		std::cout << " Hash: " << i << "  - Fee: " << i.fee << "\n\n";

	}
	std::cout << "Would you like continue the block creation? ";

	if (!Tools::cont_loop())
		return;


	this->blockchain.get_block(father_blk, this->blockchain_head);

	block.miner = this->wallet.get_compressedPublic();
	block.version = this->version;
	block.time = Tools::time_now();
	block.ID = this->blockchain_head + 1;
	block.father_hash = father_blk.work_hash;
	for (int i = 0; i < MAX_TX_IN_BLOCK && (this->confirmed_tansactions.size() != 0); i++) {
		if (this->confirmed_tansactions.size()) {
			block.transaction_list.push_back(this->confirmed_tansactions.back());
			this->confirmed_tansactions.pop_back();
		}
	}
	
	block.reward = block.compute_block_reward();
	block.find_mt_root();
	this->proof_of_work(block);

	std::cout << block << std::endl;

	if (this->validate_block(block)) {
		this->blockchain.insert_block(block);
		std::cout << "\n\n Insertion success.\n" << std::endl;
	}
	else {
		std::cout << "\n\n Insertion failed.\n" << std::endl;
	}

}

const int Node::difficulty(Block right_blk) {
	const int reach = 3;
	Block left_blk;
	int delta;
	int ceros = 4;
	if (right_blk.ID < 4)
		return ceros;
	this->blockchain.get_block(left_blk, right_blk.ID - reach);
	delta = Tools::avg_tpu(left_blk.time, right_blk.time, reach);
	if(60 - delta < 0)
		ceros += 60 - delta;
	return ceros;
}


const void Node::proof_of_work(Block& blk, int leading_ceros) {
	if(leading_ceros < 0)
		leading_ceros = difficulty(blk);
	
	std::string goal(leading_ceros, '0');
	std::string pow;
	bool mining = true;
	Block blk_cp = blk;
	blk_cp.work_hash = "";
	blk_cp.nonce = 0;
	blk_cp.transaction_list.clear();

	do {
		pow = Tools::hash_sha256(blk_cp.block_to_json(false));
		if (pow.substr(0, leading_ceros) == goal)
			mining = false;
		else
			blk_cp.nonce++;
	} while (mining);

	blk.work_hash = pow;
	blk.nonce = blk_cp.nonce;
}

const bool Node::validate_pow(Block blk) {
	std::string pow = blk.work_hash;
	blk.work_hash = "";
	blk.transaction_list.clear();
	blk.work_hash = Tools::hash_sha256(blk.block_to_json(false));

	int leading_ceros = difficulty(blk);
	std::string goal(leading_ceros, '0');
	if (pow.substr(0, leading_ceros) != goal)
		return false;

	return pow == blk.work_hash;
}

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
	std::vector<std::string> duplicates;
	
	// aboiding first tx ever
	if (tx.compute_hash() == this->origin_tx)
		return true;

	if (tx.version != "1.0.0")
		valid = false;

	// Checking duplicity
	this->blockchain.get_tx_count(duplicates, tx.hash);
	if (duplicates.size() > 0)
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
	if (tx.outputs.size() > MAX_OUT_TX)
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

	if (Tools::is_older(block.time, Tools::time_now()) < 0)
		valid = false;

	// validating PoW
	if (!this->validate_pow(block))
		valid = false;

	// validating chain continuity
	if (block.ID != 1) {
		Block father_block;
		this->blockchain.get_block(father_block, block.ID - 1);
		// Checking for father hash
		if (father_block.work_hash != block.father_hash)
			valid = false;
		// Checking for father time
		if (Tools::is_older(father_block.time, block.time) != 1)
			valid = false;
	}

	//if (block.ID < this->blockchain_head + 1)
		//valid = false;

	// validating max tx per block
	if (block.transaction_list.size() > MAX_TX_IN_BLOCK)
		valid = false;

	// validating each of the tx
	for (Transaction i : block.transaction_list) {
		if (!this->validate_tx(i))
			valid = false;
		if (Tools::is_older(i.time, block.time) < 0)
			valid = false;		
		if (Tools::avg_tpu(i.time, block.time) <= TTL)
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
	bool hit = false;


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