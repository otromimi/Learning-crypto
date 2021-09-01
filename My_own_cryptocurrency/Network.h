#pragma once

#include <vector>
#include <mutex>
#include <vector>

#include "Structures.h"
#include"DB_operations.h"
//#include "Node.h"

namespace My_own_crypto {

	

	struct Pool {
		std::vector<Transaction> valid_tx;
		unsigned int my_head;

		std::string pool_to_json(bool indent);

		void json_to_pool(std::string data);
	};

	

		void runTest();

		void runServer(DB_operations& blockchain, unsigned int& my_head, std::vector<Transaction>& confirmed_tx_pool, std::string port);

		void runClient(std::string address, std::string port, unsigned int head, std::vector<Transaction>& recieved_transacitons, std::vector<Block>& recieved_blocks);


}