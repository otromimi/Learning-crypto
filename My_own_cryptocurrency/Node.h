#pragma once

// My dependencies
#include "Structures.h"
#include "DB_operations.h"
#include "Wallet.h"
#include "Tools.h"

// Local dependencies



namespace My_own_crypto {


	class Node {
	private:
		
	public:

		Wallet wallet;

		Node(const char* wallet_name);

		const Transaction create_tx();


		const void create_block(Block& block);

		const void validate_inputs(std::string input);

		const void validate_tx(const Transaction tx);

		const void validate_block(const Block block);


	};


}