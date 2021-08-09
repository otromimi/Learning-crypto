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
#include <filesystem>
#include <sstream>
#include <cstring>


// My own dependencies
#include "Node.h"

// Local dependencies


#define MY_CRYPTO_VERSION "0.0.0" //node, block, transaction


int main()
{
    if (_DEBUG) {
        std::cout << "\n=============Checking for cryptolib=============\n";
        CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::Signer signer;
        CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::Verifier verifier;

        std::cout << signer.AlgorithmName() << ": " << signer.SignatureLength() << std::endl;
        

        CryptoPP::SHA256 test_hash;
        std::cout << "Name: " << test_hash.AlgorithmName() << std::endl;
        std::cout << "Digest size: " << test_hash.DigestSize() << std::endl;
        std::cout << "Block size: " << test_hash.BlockSize() << std::endl;

        std::cout << "\n=============Checking for sqlite3=============\n";
        DB_operations blockchain_db;

        std::cout << "\n=============Excution starts=============\n";
    }

    Transaction n_one;
    n_one.tx_id = "capricornio";

    std::cout << n_one ;
    //std::string serialized_tx = n_one.serialize_string();


    Node::create_tx(MY_CRYPTO_VERSION);




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
