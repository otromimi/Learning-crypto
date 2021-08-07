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
#include <filesystem>
#include <string>
#include <sstream>
#include <vector>


// My own dependencies
#include "Wallet.h"

// Local dependencies
#include "sha.h"
#include "eccrypto.h"
#include "sqlite3.h"


struct Txin {
    std::string TxID;
    int Txout_index;

    // Overloading operators for <<
    friend std::ostream& operator << (std::ostream& outstream, Txin& data) {
        outstream << "--------Tx-in---------" << "\nTxID: " << data.TxID << \
            "\nTx-index: " << data.Txout_index << "\n----------------------\n";
        return outstream;
    }

    friend std::ostream& operator << (std::ostream& outstream, Txin* data) {
        outstream << *data;
        return outstream;
    }

};

struct Txout {
    int value;
    CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey publicKey;

    // Overloading operators for <<
    friend std::ostream& operator << (std::ostream& outstream, Txout& data) {
        outstream << "--------Tx-out--------" << "\nValue: " << data.value << \
           "\n----------------------\n";
        return outstream;
    }

    friend std::ostream& operator << (std::ostream& outstream, Txout* data) {
        outstream << *data;
        return outstream;
    }
};


struct Transaction {

    //Fields to be hash
    std::string hash;
    std::vector<Txin> inputs;
    std::vector<Txout> outputs;

    // Overloading operators for <<
    friend std::ostream &operator << (std::ostream& outstream, Transaction& data){
        outstream << "-----Transaction: " << data.hash << "-----\n";
        for (auto aux : data.inputs) {
            outstream << aux;
        }
        outstream << "\nTxin:\n";
        for (auto aux : data.outputs) {
            outstream << aux;
        }
        outstream << "\nTout:\n";
        
        outstream << "\n----------------------\n";
        return outstream;
    }
    
    friend std::ostream& operator << (std::ostream& outstream, Transaction* data) {
        outstream << *data;
        return outstream;
    }

    // Constructor
    Transaction() {
        inputs.push_back(Txin());
        outputs.push_back(Txout());
        hash = 123;
    }

};




int main()
{

    std::cout << "=============Checking for cryptolib=============\n";
    CryptoPP::SHA256 hash;
    std::cout << "Name: " << hash.AlgorithmName() << std::endl;
    std::cout << "Digest size: " << hash.DigestSize() << std::endl;
    std::cout << "Block size: " << hash.BlockSize() << std::endl;
    std::cout << "\n";
    


    std::cout << "=============Checking for sqlite3=============\n";
    sqlite3* db;
    char* zErrMsg = 0;
    int rc;
    namespace fs = std::filesystem;
    auto db_path = fs::current_path();
    db_path += "/my_blockchain.db";

    std::cout << db_path << std::endl;

    rc = sqlite3_open("/my_blockchain.db", &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return(0);
    }
    else {
        fprintf(stderr, "Opened database successfully\n");
    }
    sqlite3_close(db);
    std::cout << "\n";



    std::cout << "=============Excution starts=============\n";

    Transaction n_one;
    

    std::cout << n_one << std::endl;
  


    Wallet my_wallet;
    
    my_wallet.test();

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
