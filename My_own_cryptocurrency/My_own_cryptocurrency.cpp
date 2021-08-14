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


// Standard dependencies
#include <iostream>


// My own dependencies
#include "Node.h"


#define MY_CRYPTO_VERSION 1 


int main()
{
   
    std::cout << "\n=============Open block_chain database=============\n";
    DB_operations blockchain_db;
    std::cout << "\n=============Creating Wallet=============\n";
    Wallet* wallet1 = new Wallet;


 
    // creating transaction
    Transaction tx(wallet1->get_compressedPublic(), 34, 4);
    tx.inputs = { "225286906970965", "225286906970965" ,"225286906970965" ,"225286906970965" ,"225286906970965" };
    tx.outputs = {Entity("225286906970965",12), Entity("225286906970965",12) ,Entity("225286906970965",12) ,Entity("225286906970965",12) };
        
    
   // signin transaction
    tx.signature = wallet1->sign_tx(tx.tx_to_json());

    //verifiying sign
    std::cout << tx << std::endl;
    bool checking = Node::sign_verifier(wallet1->get_compressedPublic(),tx.signature, tx.tx_to_json());

    

    blockchain_db.insert(Element::TRANSACTION, tx.to_db_string());

    

    delete wallet1;


    //if (_DEBUG) { //braaks in linux
    // Stoping execution in linux
    if (PLATFORM_NAME != "windows") {
        std::cout << "Platform: " << PLATFORM_NAME << std::endl;
        std::cin.get();
    }
    //}

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
