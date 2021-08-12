// My_own_cryptocurrency.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#if defined(_WIN32)
#define PLATFORM_NAME "windows" // Windows
#elif defined(_WIN64)
#define PLATFORM_NAME "windows" // Windows
#elif defined(__CYGWIN__) && !defined(_WIN32)
#define PLATFORM_NAME "windows" // Windows (Cygwin POSIX under Microsoft Window)
#elif defined(__ANDROID__)
#define PLATFORM_NAME "android" // Android (implies Linux, so it must come first)
#elif defined(__linux__)
#define PLATFORM_NAME "linux" // Debian, Ubuntu, Gentoo, Fedora, openSUSE, RedHa
#endif

#define  _CRT_SECURE_NO_WARNINGS

// Standard dependencies
#include <filesystem>
#include <sstream>
#include <cstring>


// My own dependencies
#include "Node.h"

// Local dependencies



#define MY_CRYPTO_VERSION "0.0.0" //node, block, transaction


int main()
{
    //if (_DEBUG) { //braaks in linux
        std::cout << "\n=============Checking for cryptolib=============\n";
      
        CryptoPP::SHA256 test_hash;
        std::cout << "Name: " << test_hash.AlgorithmName() << std::endl;
        std::cout << "Digest size: " << test_hash.DigestSize() << std::endl;
        std::cout << "Block size: " << test_hash.BlockSize() << std::endl;

        std::cout << "\n=============Checking for sqlite3=============\n";
        //DB_operations blockchain_db;

        std::cout << "\n=============Excution starts=============\n";
    //}

 
    //Node::create_tx(MY_CRYPTO_VERSION);

    Wallet* wallet1 = new Wallet;
    Transaction tx(MY_CRYPTO_VERSION, Node::encode(wallet1->get_publicElement()), 34, 4);
    tx.inputs = { "225286906970965", "225286906970965" ,"225286906970965" ,"225286906970965" ,"225286906970965" };
    tx.outputs = {Entity("225286906970965",12), Entity("225286906970965",12) ,Entity("225286906970965",12) ,Entity("225286906970965",12) };
        
    
    //tx = Node::create_tx(MY_CRYPTO_VERSION);
    tx.signature = Node::encode(wallet1->sign_tx(tx.tx_to_json()));

    std::cout << wallet1->verify_tx_sig(tx.signature, tx.tx_to_json()) << std::endl;
    std::cout << tx << std::endl;

    //wallet1->get_publicElement();

    bool checking = Node::sign_verifier(wallet1->get_publicElement(),decode(tx.signature), tx.tx_to_json());
    std::cout << "chenking from node: " << checking << std::endl;

    delete wallet1;

    /*
    std::time_t now = time(0);
    char* dt = ctime(&now);
    std::cout << dt << std::endl;

    std::tm* utc_struct = std::gmtime(&now);
    char* time_UTC_string = std:: asctime(utc_struct);

    std::cout << time_UTC_string << std::endl;
    */



    // Stoping execution in linux
    if (PLATFORM_NAME != "windows") {
        std::cout << "Platform: " << PLATFORM_NAME << std::endl;
        std::cin.get();
    }

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
