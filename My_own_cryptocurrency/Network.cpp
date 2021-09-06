
#include <cstdio>
#include <cstring>
#include <sstream>
#include <iostream>
#include <chrono>

#include "Network.h"
#include "struct_mapping.h"
#include "DB_operations.h"



#ifdef _WIN32
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>
#include<arpa/inet.h>
#endif


using namespace My_own_crypto;

std::mutex My_own_crypto::mutex;

//bool server_running = true;
std::atomic<bool> server_running = true;


void* get_in_addr(struct sockaddr* sa)
{
	if (sa->sa_family == AF_INET)
	{
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

#ifdef _WIN32



/// <summary>
/// Determinates if winsock is initialized
/// </summary>
/// <returns>True if it is already done</returns>
bool WinsockInitialized()
{
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET && WSAGetLastError() == WSANOTINITIALISED) {
		return false;
	}

	closesocket(s);
	return true;
}
#endif

void My_own_crypto::runServer(DB_operations& blockchain, unsigned int& my_head, std::vector<Transaction>& confirmed_tx_pool, std::string port)
{
	// Variables for writing a server. 
	/*
	1. Getting the address data structure.
	2. Openning a new socket.
	3. Bind to the socket.
	4. Listen to the socket.
	5. Accept Connection.
	6. Receive Data.
	7. Close Connection.
	*/
	int status;
	struct addrinfo hints, * res;
	int listner;

#ifdef _WIN32
	// WINDOWS
	// Initialize Winsock.
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		std::cerr << "WSAStartup failed with error: " << iResult << std::endl;
	}
#endif


	// Before using hint you have to make sure that the data structure is empty 
	memset(&hints, 0, sizeof hints);
	// Set the attribute for hint
	hints.ai_family = AF_INET; // We don't care V4 AF_INET or 6 AF_INET6
	hints.ai_socktype = SOCK_STREAM; // TCP Socket SOCK_DGRAM 
	hints.ai_flags = AI_PASSIVE;

	// Fill the res data structure and make sure that the results make sense. 
	status = getaddrinfo("127.0.0.1", port.c_str(), &hints, &res);
	if (status != 0)
	{
		std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
	}

	// Create Socket and check if error occured afterwards
	listner = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (listner < 0)
	{
		std::cerr << "socket error: " << gai_strerror(status) << std::endl;
	}

	// Bind the socket to the address of my local machine and port number 
	status = bind(listner, res->ai_addr, res->ai_addrlen);
	if (status < 0)
	{
		std::cerr << "bind: " << gai_strerror(status) << std::endl;
	}

	status = listen(listner, 10);
	if (status < 0)
	{
		std::cerr << "listen: " << gai_strerror(status) << std::endl;
	}

	// Free the res linked list after we are done with it	
	freeaddrinfo(res);


	
	int new_conn_fd;
	struct sockaddr_storage client_addr;
	socklen_t addr_size;
	char s[INET6_ADDRSTRLEN]; // an empty string 

	// Calculate the size of the data structure	
	addr_size = sizeof client_addr;

	//std::cout << "I am now accepting connections ...\n";


	char* buffer; 
	std::stringstream readStream;
	bool readData = true;
	int readResult;
	Block blk;
	Pool tx_pool;


	std::string his_h_str;
	std::string size_blk;

	std::string str_pool;
	std::string size_str_pool;
	
	while (server_running) {

		buffer = (char*)malloc(32);
		memset(buffer, 0, 32); // buffer to ceros.


		// Accept a new connection and retrieve file descriptor for it.
		new_conn_fd = accept(listner, (struct sockaddr*)&client_addr, &addr_size);
		if (new_conn_fd < 0)
		{
			std::cerr << "accept: " << gai_strerror(new_conn_fd) << std::endl;
			continue;
		}

		// Get connections info
		inet_ntop(client_addr.ss_family, get_in_addr((struct sockaddr*)&client_addr), s, sizeof s);
		//std::cout << "I am now connected to " << s << std::endl;


		//Get updated pool
		mutex.lock();
		tx_pool.valid_tx = confirmed_tx_pool;
		mutex.unlock();

		str_pool = tx_pool.pool_to_json(false);
		size_str_pool = std::to_string(str_pool.size());

		// Send poll size and then pool
		status = send(new_conn_fd, size_str_pool.c_str(), size_str_pool.size(), 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		status = send(new_conn_fd, str_pool.c_str(), str_pool.size(), 0);


		// Getting his head possitoin on the blockchain
		readResult = recv(new_conn_fd, buffer, 32, 0);
		buffer[32 - 1] = 0;
		readStream << buffer;
		unsigned long his_head = std::stoul(readStream.str());

		free(buffer);// Controling memory leaks

		
		while (his_head < my_head) {
			his_head++;

			// Get block
			mutex.lock();
			blockchain.get_block(blk, his_head);
			mutex.unlock();
			//std::cout << blk << std::endl;

			// Prepare block data for sending
			his_h_str = blk.block_to_json(false);
			size_blk = std::to_string(his_h_str.size());
			

			// Send block
			status = send(new_conn_fd, size_blk.c_str(), size_blk.size(), 0);
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			status = send(new_conn_fd, his_h_str.c_str(), his_h_str.size(), 0);
			
		}
		
#ifdef _WIN32

		closesocket(new_conn_fd);
#else

		close(new_conn_fd);
#endif

	}


	// Close the socket before we finish 
#ifdef _WIN32
	
	closesocket(listner);
#else
	
	close(listner);
#endif

#ifdef _WIN32
	WSACleanup();
#endif

	
}

std::string Pool::pool_to_json(bool indent) {

	struct_mapping::reg(&Entity::account, "Account");
	struct_mapping::reg(&Entity::value, "Value");

	struct_mapping::reg(&Transaction::version, "Version");
	struct_mapping::reg(&Transaction::time, "Time");
	struct_mapping::reg(&Transaction::inputs, "Inputs");
	struct_mapping::reg(&Transaction::outputs, "Outputs");
	struct_mapping::reg(&Transaction::origin, "Origin");
	struct_mapping::reg(&Transaction::fee, "Fee");
	struct_mapping::reg(&Transaction::signature, "Signature");

	struct_mapping::reg(&Pool::valid_tx, "tx_pool");

	std::ostringstream pool_json;
	if (indent)
		struct_mapping::map_struct_to_json(Pool::valid_tx, pool_json, "  ");
	else
		struct_mapping::map_struct_to_json(Pool::valid_tx, pool_json);


	return pool_json.str();
}

void Pool::json_to_pool(std::string data) {

	struct_mapping::reg(&Entity::account, "Account");
	struct_mapping::reg(&Entity::value, "Value");

	struct_mapping::reg(&Transaction::version, "Version");
	struct_mapping::reg(&Transaction::time, "Time");
	struct_mapping::reg(&Transaction::inputs, "Inputs");
	struct_mapping::reg(&Transaction::outputs, "Outputs");
	struct_mapping::reg(&Transaction::origin, "Origin");
	struct_mapping::reg(&Transaction::fee, "Fee");
	struct_mapping::reg(&Transaction::signature, "Signature");

	struct_mapping::reg(&Pool::valid_tx, "tx_pool");

	std::istringstream jason_data(data);

	struct_mapping::map_json_to_struct(*this, jason_data);
}


void My_own_crypto::runClient(std::string address, std::string port, unsigned int head, std::vector<Transaction>& recieved_transacitons, std::vector<Block>& recieved_blocks)
{
	// Variables for writing a client. 
	/*
	1. Getting the address data structure.
	2. Openning a new socket.
	3. Conect to a server.
	4. Recieve data.
	5. Close connection.
	*/
	int status;
	struct addrinfo hints, * res;
	int client_request;

#ifdef _WIN32
	//----------------------
	// Initialize Winsock.
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		std::cerr << "WSAStartup failed with error: " << iResult << std::endl;
	}
#endif


	// Before using hint you have to make sure that the data structure is empty 
	memset(&hints, 0, sizeof hints);
	// Set the attribute for hint
	hints.ai_family = AF_INET; // IPV4
	hints.ai_socktype = SOCK_STREAM; // TCP 
	hints.ai_flags = AI_PASSIVE;

	//sockaddr_in clientService;
	//clientService.sin_family = AF_INET;
	//clientService.sin_addr.s_addr = inet_addr("127.0.0.1");
	//clientService.sin_port = htons(27015);

	//hints = (addrinfo*)&clientService;

	// Fill the res data structure and make sure that the results make sense. 
	status = getaddrinfo(address.c_str(), port.c_str(), &hints, &res);
	if (status != 0)
	{
		std::cerr << "getaddrinfo error : " << gai_strerror(status) << std::endl;
	}

	// Create Socket and check if error occured afterwards
	client_request = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (client_request < 0)
	{
		std::cerr << "socket error: " << gai_strerror(status) << std::endl;
	}

	// Connect the socket to the address of my local machine and port number 
	status = connect(client_request, res->ai_addr, res->ai_addrlen);
	if (status < 0)
	{
		std::cerr << "connect: " << gai_strerror(status) << std::endl;
	}


	char* buffer = (char*)malloc(32 + 1);
	std::stringstream readStream;
	int readData;

	int out = 0;
	int sendResult;


	// more sperimenting
	memset(buffer, 0, 32);

	int readResult = recv(client_request, buffer, 32, 0);
	buffer[32] = NULL;
	readStream << buffer;

	readData = readStream.str().find("end;") == std::string::npos;

	std::cout << "Done tx_valids_size (length: " << readStream.str().length() << ") " << readStream.str() << std::endl;

	/// speriment

	unsigned long new_size = 30;
	try {
		new_size = std::stoul(readStream.str());
	}
	catch (std::exception e) {
		new_size = 0;
	}


	free(buffer);		
	buffer = (char*)malloc(new_size + 1);
	memset(buffer, 0, new_size);

	readResult = recv(client_request, buffer, new_size, 0);
	buffer[new_size] = NULL;

	if (new_size > 2) {


		std::cout << readStream.str() << std::endl;
		readStream.str("");

		readStream << buffer;


		readData = readStream.str().find("end;") == std::string::npos;
		std::cout.flush();
		std::cout << "Done tx_valids (length: " << readStream.str().length() << ") " << readStream.str() << std::endl;

	
		Pool recieved_pool;
		recieved_pool.json_to_pool("{\"tx_pool\":" + readStream.str() + "}");
		mutex.lock();
		recieved_transacitons = recieved_pool.valid_tx;
		mutex.unlock();
	}
	
	sendResult = send(client_request, std::to_string(head).c_str(), 30, 0);

	do{
		buffer = (char*)malloc(32 + 1);
		readStream.str("");
		memset(buffer, 0, 32);

		readResult = recv(client_request, buffer, 32, 0);

		if (readResult > 0) {
			buffer[32] = 0;
			readStream << buffer;

			readData = readStream.str().find("end;") == std::string::npos;

			std::cout << "Done block_size(length: " << readStream.str().length() << ") " << readStream.str() << std::endl;

			// Setting recieving structures for block
			try {
				new_size = std::stoul(readStream.str());
			}
			catch (std::exception e) {
				new_size = 0;
			}

			if (new_size > 2) {

				free(buffer);
				buffer = (char*)malloc(new_size + 1);
				memset(buffer, 0, new_size);

				readResult = recv(client_request, buffer, new_size, 0);
				buffer[new_size] = NULL;




				std::cout << readStream.str() << std::endl;
				readStream.str("");

				readStream << buffer;
				readStream << std::endl;


				readData = readStream.str().find("end;") == std::string::npos;
				std::cout.flush();
				std::cout << "Done block (length: " << readStream.str().length() << ") " << readStream.str() << std::endl;

				
				mutex.lock();
				Block new_blk;
				new_blk.json_to_block(readStream.str());
				recieved_blocks.push_back(new_blk);
				mutex.unlock();
				
			}
			free(buffer);
		}

		
	} while (readResult > 0);

	// Close the socket before we finish 
#ifdef _WIN32
	closesocket(client_request);
#else
	close(client_request);
#endif

#ifdef _WIN32
	WSACleanup();
#endif

}



void My_own_crypto::runTest() {

	//Server::mutex.lock();
	for (int i = 0; i < 102; i++)
		std::cout << "#";
	
	//Server::mutex.unlock();

}