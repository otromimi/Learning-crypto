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


using namespace My_own_crypto;

#define MY_CRYPTO_VERSION 1 



int main()
{
   
    std::cout << "\n=============Open block_chain database=============\n";
    //DB_operations blockchain_db;
    std::cout << "\n=============Creating Wallet=============\n";

    
    Node node1("peter");
   

    
 
    // creating transaction
    Transaction tx1(Tools::time_now(), node1.wallet.get_compressedPublic(), 42.32f);
    tx1.inputs = { "225286906970965", "225286906970965" ,"225286906970965" ,"225286906970965" ,"225286906970965" };
    tx1.outputs = {Entity("225286906970965",12), Entity("225286906970965",12) ,Entity("225286906970965",12) ,Entity("225286906970965",12) };

    Transaction tx2(Tools::time_now(), node1.wallet.get_compressedPublic(), 422.f);
    tx2.inputs = { "225286906970965", "225286906970965" };
    tx2.outputs = { Entity("225286906970965",1122), Entity("225286906970965",1122) ,Entity("225286906970965",12)};

    Transaction tx3(Tools::time_now(), node1.wallet.get_compressedPublic(), 4.3f);
    tx3.inputs = { "225286906970965"};
    tx3.outputs = { Entity("225286906970965",12), Entity("225286906970965",12) };

    Transaction tx4(Tools::time_now(), node1.wallet.get_compressedPublic(), 100.f);
    tx4.inputs = { "225286906970965", "225286906970965" ,"225286906970965" ,"225286906970965" ,"225286906970965" };
    tx4.outputs = { Entity("225286943970965",142) };

    

    
    
   
    
   // signin transaction
    tx1.signature = node1.wallet.sign_tx(tx1.tx_to_json(false, false));
    tx2.signature = node1.wallet.sign_tx(tx2.tx_to_json(false, false));
    tx3.signature = node1.wallet.sign_tx(tx3.tx_to_json(false, false));
    tx4.signature = node1.wallet.sign_tx(tx4.tx_to_json(false, false));


    Block b1;

    b1.transaction_list.push_back(tx1);
    b1.transaction_list.push_back(tx2);
    b1.transaction_list.push_back(tx3);
    b1.transaction_list.push_back(tx4);

    b1.find_mt_root();








    //verifiying sign
    //std::cout << tx << std::endl;
    //bool checking = Tools::sign_verifier(node1.wallet.get_compressedPublic(),tx.signature, tx.tx_to_json(false, false));

    

    //blockchain_db.insert(Element::TRANSACTION, tx.to_db_string());
    //blockchain_db.select(Element::TRANSACTION, tx.to_db_string());

    //Transaction tx = node1.create_tx();

    //std::cout << tx.tx_to_json(true, true) << std::endl;

   

    

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

