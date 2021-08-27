
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
		wprintf(L"WSAStartup failed with error: %ld\n", iResult);
		
	}
#endif


	// Before using hint you have to make sure that the data structure is empty 
	memset(&hints, 0, sizeof hints);
	// Set the attribute for hint
	hints.ai_family = AF_INET; // We don't care V4 AF_INET or 6 AF_INET6
	hints.ai_socktype = SOCK_STREAM; // TCP Socket SOCK_DGRAM 
	hints.ai_flags = AI_PASSIVE;

	// Fill the res data structure and make sure that the results make sense. 
	status = getaddrinfo("127.0.0.1", "5757", &hints, &res);
	if (status != 0)
	{
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
	}

	// Create Socket and check if error occured afterwards
	listner = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (listner < 0)
	{
		fprintf(stderr, "socket error: %s\n", gai_strerror(status));
	}

	// Bind the socket to the address of my local machine and port number 
	status = bind(listner, res->ai_addr, res->ai_addrlen);
	if (status < 0)
	{
		fprintf(stderr, "bind: %s\n", gai_strerror(status));
	}

	status = listen(listner, 10);
	if (status < 0)
	{
		fprintf(stderr, "listen: %s\n", gai_strerror(status));
	}

	// Free the res linked list after we are done with it	
	freeaddrinfo(res);


	// We should wait now for a connection to accept
	int new_conn_fd;
	struct sockaddr_storage client_addr;
	socklen_t addr_size;
	char s[INET6_ADDRSTRLEN]; // an empty string 

							  // Calculate the size of the data structure	
	addr_size = sizeof client_addr;

	printf("I am now accepting connections ...\n");


	char* buffer;
	buffer = (char*)malloc(32);
	std::stringstream readStream;
	bool readData = true;
	int readResult;
	Block blk;
	
	memset(buffer, 0, 32);

	//while (1) {
		// Accept a new connection and return back the socket desciptor 
		new_conn_fd = accept(listner, (struct sockaddr*)&client_addr, &addr_size);
		if (new_conn_fd < 0)
		{
			fprintf(stderr, "accept: %s\n", gai_strerror(new_conn_fd));
			//continue;
		}


		inet_ntop(client_addr.ss_family, get_in_addr((struct sockaddr*)&client_addr), s, sizeof s);
		printf("I am now connected to %s \n", s);

		

		Pool tx_pool;
		mutex.lock();
		tx_pool.valid_tx = confirmed_tx_pool;
		mutex.unlock();

		std::string str_pool = tx_pool.pool_to_json(false);
		std::string size_str_pool = std::to_string(str_pool.size());

		status = send(new_conn_fd, size_str_pool.c_str(), size_str_pool.size(), 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		status = send(new_conn_fd, str_pool.c_str(), str_pool.size(), 0);

		std::cout << str_pool.size() << std::endl;

		readResult = recv(new_conn_fd, buffer, 32, 0);
		buffer[32 - 1] = 0;
		readStream << buffer;
		unsigned long his_head = std::stoul(readStream.str());
		std::cout << his_head << std::endl;

		his_head++;
		while (his_head < my_head+1) {
			mutex.lock();
			blockchain.get_block(blk, his_head);
			mutex.unlock();
			std::cout << blk << std::endl;
			
			std::string his_h_str = blk.block_to_json(false);
			std::string size_blk = std::to_string(his_h_str.size());
			std::cout << his_h_str << std::endl;

			status = send(new_conn_fd, size_blk.c_str(), size_blk.size(), 0);
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			status = send(new_conn_fd, his_h_str.c_str(), his_h_str.size(), 0);
			his_head++;
		}

		/*do {
			readResult = recv(new_conn_fd, buffer, 30, 0);
			readStream << buffer;


			readData = readStream.str().find("end;") == std::string::npos;

			std::cout << "Done (length: " << readStream.str().length() << ") " << readStream.str() << std::endl;
		} while (readResult);*/



		if (status == -1)
		{
#ifdef _WIN32
			closesocket(new_conn_fd);
			WSACleanup();
#else
			close(new_conn_fd);
#endif
			_exit(4);
		}

	//}
	// Close the socket before we finish 
#ifdef _WIN32
	closesocket(new_conn_fd);
#else
	close(new_conn_fd);
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


void runClient(std::string address, std::string port)
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
	int listner;

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

	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	//clientService.sin_addr.s_addr = inet_addr("127.0.0.1");
	clientService.sin_port = htons(27015);

	//hints = (addrinfo*)&clientService;

	// Fill the res data structure and make sure that the results make sense. 
	status = getaddrinfo("127.0.0.1", "5757", &hints, &res);
	if (status != 0)
	{
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
	}

	// Create Socket and check if error occured afterwards
	listner = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (listner < 0)
	{
		fprintf(stderr, "socket error: %s\n", gai_strerror(status));
	}

	// Bind the socket to the address of my local machine and port number 
	status = connect(listner, res->ai_addr, res->ai_addrlen);
	if (status < 0)
	{
		fprintf(stderr, "connect: %s\n", gai_strerror(status));
	}

	std::cout << res->ai_addr << std::endl;

	char* buffer;
	buffer = (char*)malloc(8);
	std::stringstream readStream;
	int readData;

	memset(buffer, 0, 8);

	int readResult = recv(listner, buffer, 8, 0);
	buffer[8] = NULL;
	readStream << buffer;


	readData = readStream.str().find("end;") == std::string::npos;

	std::cout << "Done (length: " << readStream.str().length() << ") " << readStream.str() << std::endl;



	readResult = recv(listner, buffer, 8, 0);

	//buffer = (char*)peter;

	int sendResult = send(listner, "caracolitas de colores", 30, 0);



	delete buffer;

	// Close the socket before we finish 
#ifdef _WIN32
	closesocket(listner);
#else
	close(new_conn_fd);
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