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

#define MY_CRYPTO_VERSION "1.0.0"



int main()
{
   
    std::cout << "\n=============Open block_chain database=============\n";
    DB_operations blockchain_db;
    std::cout << "\n=============Creating Wallet=============\n";

    
    Node node1("User3");
   
 
    // First transaction (primigine transaciton), the one that puts founds on the different accounts.
    Transaction tx1("2021/7/23 18:40:25", "02d3df2c1aeedb13b9a29af0e3d42a4f19ac3187b49377c78fe9b7844c69bf1ea3h", 0.0f);
    tx1.version = MY_CRYPTO_VERSION;
    tx1.inputs = { "0000000000000000000000000000000000000000000000000000000000000000" };
    tx1.outputs = {Entity("02d3df2c1aeedb13b9a29af0e3d42a4f19ac3187b49377c78fe9b7844c69bf1ea3h",1000.0f), 
        Entity("0390562eed62f9563117ffb2fa3b2e8814b901571cbbb6a2ac443afb777b299e67h",1000.0f), 
        Entity("037e139da7567349dc47cfa32fec5774556addfdf110fd8eedae09f9ca678b0033h",1000.0f), 
        Entity("03d393e113c107d6aa1b710623f65ddb6abe7179a8ab595ce7587ad97b43acf193h",1000.0f) };

    Transaction tx2("2021/7/23 18:40:30", "0390562eed62f9563117ffb2fa3b2e8814b901571cbbb6a2ac443afb777b299e67h", 2.f);
    tx2.version = MY_CRYPTO_VERSION;
    tx2.inputs = { "9AF214EA52CC156C4AFDFF520E0A308462E7139A0FB72FFA9B210FC21FD31ADD" };
    tx2.outputs = { Entity("02d3df2c1aeedb13b9a29af0e3d42a4f19ac3187b49377c78fe9b7844c69bf1ea3h",10.0f),
         Entity("0390562eed62f9563117ffb2fa3b2e8814b901571cbbb6a2ac443afb777b299e67h",698.0f),
         Entity("037e139da7567349dc47cfa32fec5774556addfdf110fd8eedae09f9ca678b0033h",10.0f),
         Entity("03d393e113c107d6aa1b710623f65ddb6abe7179a8ab595ce7587ad97b43acf193h",10.0f) };

    Transaction tx3("2021/7/23 18:41:30",  "037e139da7567349dc47cfa32fec5774556addfdf110fd8eedae09f9ca678b0033h", 1.f);
    tx3.version = MY_CRYPTO_VERSION;
    tx3.inputs = { "E65134724F9B0FE10F4D092470068A5954DC85014E7BF2F556A70C5C7B84C3AD"};
    tx3.outputs = { Entity("02d3df2c1aeedb13b9a29af0e3d42a4f19ac3187b49377c78fe9b7844c69bf1ea3h",2.0f),
        Entity("0390562eed62f9563117ffb2fa3b2e8814b901571cbbb6a2ac443afb777b299e67h",3.0f),
        Entity("037e139da7567349dc47cfa32fec5774556addfdf110fd8eedae09f9ca678b0033h",1.0f),
        Entity("03d393e113c107d6aa1b710623f65ddb6abe7179a8ab595ce7587ad97b43acf193h",3.0f) };


    Transaction tx4("2021/7/23 19:41:30", "037e139da7567349dc47cfa32fec5774556addfdf110fd8eedae09f9ca678b0033h", 1.5f);
    tx4.version = MY_CRYPTO_VERSION;
    tx4.inputs = { "9AF214EA52CC156C4AFDFF520E0A308462E7139A0FB72FFA9B210FC21FD31ADD" };
    tx4.outputs = { Entity("02d3df2c1aeedb13b9a29af0e3d42a4f19ac3187b49377c78fe9b7844c69bf1ea3h",298.5f),
        Entity("0390562eed62f9563117ffb2fa3b2e8814b901571cbbb6a2ac443afb777b299e67h",500.0f),
        Entity("037e139da7567349dc47cfa32fec5774556addfdf110fd8eedae09f9ca678b0033h",100.0f),
        Entity("03d393e113c107d6aa1b710623f65ddb6abe7179a8ab595ce7587ad97b43acf193h",100.0f) };


   

    
   
   
    // signin transaction
    tx1.signature = "0225A040F55FDB0FF88EA7BB7D23226DBE7E077010D859B845C2639A6D22CE2D8E24AF8E2432F55F13FC801BC5626781927BB1CF38DE366E814ACA344F081B86";
    tx2.signature = "9745CBF55B031DBE80B8603EC6791AEB0ECAC14A31A4271EA7AEB6628051A52D5F7C7B8B2C02A4394AC95B595884D0841D4542987AD50809A968A068E4559106";
    tx3.signature = "85FF27C931CAE0FFEDA0A423E04984D9364A0CD6C092A0C9D4C8E41DE11364F483BC6661FE121C1BBD87E778D35AD96DDE83BF314D48E084A50F5DE968174056";
    tx4.signature = "258E41868D3836BD87E34BEFB6EEBB58F53B59529573438EEC250FA8DD17D5A2BC8753FBB5C443EE0AAEA1B57F36F8991595C348C500714DA676A5A030DF8664";

    //node1.wallet.sign_tx(tx3.tx_to_json());
    //tx3.signature = node1.wallet.sign_tx(tx3.tx_to_json());
    //std::cout << tx3.signature << std::endl;
    //std::cout << node1.wallet.get_compressedPublic() << std::endl;
    std::cout << Tools::sign_verifier(tx1.origin, tx1.signature, tx1.tx_to_json()) << std::endl;
    std::cout << Tools::sign_verifier(tx2.origin, tx2.signature, tx2.tx_to_json()) << std::endl;
    std::cout << Tools::sign_verifier(tx3.origin, tx3.signature, tx3.tx_to_json()) << std::endl;
    std::cout << Tools::sign_verifier(tx4.origin, tx4.signature, tx4.tx_to_json()) << std::endl;
    

    Block block("CEB8B8417BE86B68548638830E7C3DF51A425EE1322D8D719944C09040405FC5");
    block.version = MY_CRYPTO_VERSION;
    block.ID = 0;
    block.father_hash = "000000000000000000000000000000000000000000000000";
    block.transaction_list = { tx1, tx2, tx3, tx4 };
    block.reward = block.compute_block_reward();

    block.find_mt_root();

    block.work_hash = Tools::hash_sha256(block.block_to_json());

    //  std::cout << block.block_to_json(true, true) << std::endl;

    //Tools::to_txt("block_test1", block.block_to_json(true, true));

    blockchain_db.insert_block(block);

    //std::cout << Tools::get_int_time(block.time).tm_year << std::endl;



    std::cout << block.block_to_json(true) << std::endl;

    std::cout << "...ea3h = " << blockchain_db.get_balance("02d3df2c1aeedb13b9a29af0e3d42a4f19ac3187b49377c78fe9b7844c69bf1ea3h") << std::endl;
    std::cout << "...e67h = " << blockchain_db.get_balance("0390562eed62f9563117ffb2fa3b2e8814b901571cbbb6a2ac443afb777b299e67h") << std::endl;
    std::cout << "...033h = " << blockchain_db.get_balance("037e139da7567349dc47cfa32fec5774556addfdf110fd8eedae09f9ca678b0033h") << std::endl;
    std::cout << "...193h = " << blockchain_db.get_balance("03d393e113c107d6aa1b710623f65ddb6abe7179a8ab595ce7587ad97b43acf193h") << std::endl;
   

    
    

   
    //verifiying sign
    //std::cout << tx << std::endl;
    //bool checking = Tools::sign_verifier(node1.wallet.get_compressedPublic(),tx.signature, tx.tx_to_json(false, false));

    

    //blockchain_db.insert(Element::TRANSACTION, tx.to_db_string());
    //blockchain_db.select(Element::TRANSACTION, tx1.to_db_string());

    //Transaction tx = node1.create_tx();



   

    

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

