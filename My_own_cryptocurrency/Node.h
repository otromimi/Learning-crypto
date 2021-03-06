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
		
		std::string origin_tx = "88A7A5CC39A111E1BB0931DD79D083611C03180258F06AEFE7CE1421B20DAE0E";

		int min_per_block;
		//std::vector<Transaction> confirmed_tx_pool;

	public:

		std::string version;

		Wallet wallet;
		DB_operations blockchain;
		unsigned int blockchain_head;

		std::vector<Transaction> confirmed_tansactions;

		std::vector<Transaction> received_transactions;
		std::vector<Block> received_blocks;

		Node(const char* wallet_name, const char* version);
		~Node();

		/// <summary>
		/// Runs the server in a detached thread.
		/// </summary>
		/// <param name="port">Port were the server is gonna be run.</param>
		void run_server(std::string port);

		
		/// <summary>
		/// Retrieves updated informaiton from its pear.
		/// </summary>
		/// <param name="port">Port were the pear server is running.</param>
		void check_node(std::string port);

		/// <summary>
		/// Creates transaction in interactive mode.
		/// </summary>
		/// <returns>New created transaction.</returns>
		void create_tx();

		const void create_block();

		const int difficulty(Block blk);

		const void proof_of_work(Block& blk, int leading_ceros = -1);

		const bool validate_pow(Block blk);

		const float validate_inputs(std::vector<std::string> inputs, std::string address, std::string time);


		const bool validate_sign(Transaction tx);

		/// <summary>
		/// Validates transaction integrity and legitimity.
		/// </summary>
		/// <param name="tx">Transaction to be check.</param>
		/// <returns></returns>
		const bool validate_tx(Transaction tx);

		const bool validate_block(Block block);

		const void print_block();

		const void print_transaction();

		const void print_wallet_info();

	};


}