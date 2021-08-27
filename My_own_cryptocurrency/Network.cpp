
#include<cstdio>
#include<cstring>
#include <sstream>
#include <iostream>
#include <thread>
#include<chrono>

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

std::mutex mutex;


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

	std::cout << "I am now accepting connections ...\n";


	char* buffer = (char*)malloc(32);
	std::stringstream readStream;
	bool readData = true;
	int readResult;
	Block blk;
	Pool tx_pool;
	
	memset(buffer, 0, 32); // buffer to ceros.

	
	// Accept a new connection and retrieve file descriptor for it.
	new_conn_fd = accept(listner, (struct sockaddr*)&client_addr, &addr_size);
	if (new_conn_fd < 0)
	{
		std::cerr << "accept: " << gai_strerror(new_conn_fd) << std::endl;
		//continue;
	}

	// Get connections info
	inet_ntop(client_addr.ss_family, get_in_addr((struct sockaddr*)&client_addr), s, sizeof s);
	std::cout << "I am now connected to " << s << std::endl;

		
	//Get updated pool
	mutex.lock();
	tx_pool.valid_tx = confirmed_tx_pool;
	mutex.unlock();

	std::string str_pool = tx_pool.pool_to_json(false);
	std::string size_str_pool = std::to_string(str_pool.size());

	// Send poll size and then pool
	status = send(new_conn_fd, size_str_pool.c_str(), size_str_pool.size(), 0);
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	status = send(new_conn_fd, str_pool.c_str(), str_pool.size(), 0);

	//std::cout << str_pool.size() << std::endl;

	// Getting his head possitoin on the blockchain
	readResult = recv(new_conn_fd, buffer, 32, 0);
	buffer[32 - 1] = 0;
	readStream << buffer;
	unsigned long his_head = std::stoul(readStream.str());
	std::cout << his_head << std::endl;

	free(buffer);// Controling memory leaks

	his_head++;
	while (his_head < my_head+1) {

		// Get block
		mutex.lock();
		blockchain.get_block(blk, his_head);
		mutex.unlock();
		std::cout << blk << std::endl;
		
		// Prepare block data for sending
		std::string his_h_str = blk.block_to_json(false);
		std::string size_blk = std::to_string(his_h_str.size());
		//std::cout << his_h_str << std::endl;

		// Send block
		status = send(new_conn_fd, size_blk.c_str(), size_blk.size(), 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		status = send(new_conn_fd, his_h_str.c_str(), his_h_str.size(), 0);
		his_head++;
	}


	// Close the socket before we finish 
#ifdef _WIN32
	closesocket(new_conn_fd);
	closesocket(listner);
#else
	close(new_conn_fd);
	close(listner);
#endif

#ifdef _WIN32
	WSACleanup();
#endif

	
}

std::string Pool::pool_to_json(bool indent) {

	

	struct_mapping::reg(&Pool::valid_tx, "tx_pool");

	std::ostringstream pool_json;
	if (indent)
		struct_mapping::map_struct_to_json(Pool::valid_tx, pool_json, "  ");
	else
		struct_mapping::map_struct_to_json(Pool::valid_tx, pool_json);


	return pool_json.str();
}

void Pool::json_to_pool(std::string data) {



	struct_mapping::reg(&Pool::valid_tx, "tx_pool");

	std::istringstream jason_data(data);

	struct_mapping::map_json_to_struct(*this, jason_data);
}


void My_own_crypto::runClient(std::string address, std::string port, int head)
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
		wprintf(L"WSAStartup failed with error: %ld\n", iResult);

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
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
	}

	// Create Socket and check if error occured afterwards
	client_request = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (client_request < 0)
	{
		fprintf(stderr, "socket error: %s\n", gai_strerror(status));
	}

	// Bind the socket to the address of my local machine and port number 
	status = connect(client_request, res->ai_addr, res->ai_addrlen);
	if (status < 0)
	{
		fprintf(stderr, "connect: %s\n", gai_strerror(status));
	}

	std::cout << res->ai_addr << std::endl;

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

	std::cout << "Done (length: " << readStream.str().length() << ") " << readStream.str() << std::endl;

	/// speriment

	unsigned long new_size = 30;
	try {
		new_size = std::stoul(readStream.str());
	}
	catch (std::exception e) {

	}

	free(buffer);
	buffer = (char*)malloc(new_size + 1);
	memset(buffer, 0, new_size);

	readResult = recv(client_request, buffer, new_size, 0);
	buffer[new_size] = NULL;




	std::cout << readStream.str() << std::endl;
	readStream.str("");

	readStream << buffer;


	readData = readStream.str().find("end;") == std::string::npos;
	std::cout.flush();
	std::cout << "Done (length: " << readStream.str().length() << ") " << readStream.str() << std::endl;

	sendResult = send(client_request, std::to_string(head).c_str(), 30, 0);

	readStream.str("");
	// more sperimenting
	memset(buffer, 0, 32);

	readResult = recv(client_request, buffer, 32, 0);
	buffer[32] = 0;
	readStream << buffer;

	readData = readStream.str().find("end;") == std::string::npos;

	std::cout << "Done (length: " << readStream.str().length() << ") " << readStream.str() << std::endl;

	/// speriment


	new_size = std::stoul(readStream.str());

	free(buffer);
	buffer = (char*)malloc(new_size + 1);
	memset(buffer, 0, new_size);

	readResult = recv(client_request, buffer, new_size, 0);
	buffer[new_size] = NULL;




	std::cout << readStream.str() << std::endl;
	readStream.str("");

	readStream << buffer;


	readData = readStream.str().find("end;") == std::string::npos;
	std::cout.flush();
	std::cout << "Done (length: " << readStream.str().length() << ") " << readStream.str() << std::endl;

	free(buffer);

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