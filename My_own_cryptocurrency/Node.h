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
		DB_operations blockchain;
		unsigned int blockchain_head;

		Node(const char* wallet_name);


		/// <summary>
		/// Creates transaction in interactive mode.
		/// </summary>
		/// <returns>New created transaction.</returns>
		const Transaction create_tx();

		const void create_block(Block& block);

		const float validate_inputs(std::vector<std::string> inputs, std::string address);

		/// <summary>
		/// Validates transaction integrity and legitimity.
		/// </summary>
		/// <param name="tx">Transaction to be check.</param>
		/// <returns></returns>
		const bool validate_tx(Transaction tx);

		const void validate_block(const Block block);


	};


}