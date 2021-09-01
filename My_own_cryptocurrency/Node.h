#pragma once

// My dependencies
#include "Structures.h"
#include "DB_operations.h"
#include "Wallet.h"
#include "Tools.h"
#include "Network.h"

// Local dependencies



namespace My_own_crypto {


	class Node {
	private:
		
		int min_per_block;
		//std::vector<Transaction> confirmed_tx_pool;

	public:

		Wallet wallet;
		DB_operations blockchain;
		unsigned int blockchain_head;

		std::vector<Transaction> received_transactions;
		std::vector<Block> received_blocks;

		Node(const char* wallet_name);
		~Node();

		//runClient(std::string address, std::string port, int head);
		void check_node(std::string port);

		/// <summary>
		/// Creates transaction in interactive mode.
		/// </summary>
		/// <returns>New created transaction.</returns>
		const Transaction create_tx();

		const void create_block(Block& block);

		const float validate_inputs(std::vector<std::string> inputs, std::string address, std::string time);


		const bool validate_sign(Transaction tx);

		/// <summary>
		/// Validates transaction integrity and legitimity.
		/// </summary>
		/// <param name="tx">Transaction to be check.</param>
		/// <returns></returns>
		const bool validate_tx(Transaction tx);

		const bool validate_block(Block block);



	};


}