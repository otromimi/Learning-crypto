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
      
    std::cout << "\n=============Creating Wallet=============\n";

    
    Node node1("User1");
   
 
    // First transaction (primigine transaciton), the one that puts founds on the different accounts.
    Transaction tx1("2021-07-23 18:40:25", "02d3df2c1aeedb13b9a29af0e3d42a4f19ac3187b49377c78fe9b7844c69bf1ea3h", 0.0f);
    tx1.version = MY_CRYPTO_VERSION;
    tx1.inputs = { "0000000000000000000000000000000000000000000000000000000000000000" };
    tx1.outputs = {Entity("02d3df2c1aeedb13b9a29af0e3d42a4f19ac3187b49377c78fe9b7844c69bf1ea3h",1000.0f), 
        Entity("0390562eed62f9563117ffb2fa3b2e8814b901571cbbb6a2ac443afb777b299e67h",1000.0f), 
        Entity("037e139da7567349dc47cfa32fec5774556addfdf110fd8eedae09f9ca678b0033h",1000.0f), 
        Entity("03d393e113c107d6aa1b710623f65ddb6abe7179a8ab595ce7587ad97b43acf193h",1000.0f) };

    Transaction tx2("2021-07-23 18:40:30", "0390562eed62f9563117ffb2fa3b2e8814b901571cbbb6a2ac443afb777b299e67h", 2.f);
    tx2.version = MY_CRYPTO_VERSION;
    tx2.inputs = { "477ED9817BCA3D870CF4FB06BA26951CFC865B6B4641D4C85BD8A30F006BFD6D" };
    tx2.outputs = { Entity("02d3df2c1aeedb13b9a29af0e3d42a4f19ac3187b49377c78fe9b7844c69bf1ea3h",100.0f),
         Entity("0390562eed62f9563117ffb2fa3b2e8814b901571cbbb6a2ac443afb777b299e67h",698.0f),
         Entity("037e139da7567349dc47cfa32fec5774556addfdf110fd8eedae09f9ca678b0033h",100.0f),
         Entity("03d393e113c107d6aa1b710623f65ddb6abe7179a8ab595ce7587ad97b43acf193h",100.0f) };

    Transaction tx3("2021-07-23 18:41:30",  "037e139da7567349dc47cfa32fec5774556addfdf110fd8eedae09f9ca678b0033h", 1.f);
    tx3.version = MY_CRYPTO_VERSION;
    tx3.inputs = { "8CB375B65058192BE8714C660D4AC9810B0AE6EC4C8595F0F6795EA120B333D4"};
    tx3.outputs = { Entity("02d3df2c1aeedb13b9a29af0e3d42a4f19ac3187b49377c78fe9b7844c69bf1ea3h",2.0f),
        Entity("0390562eed62f9563117ffb2fa3b2e8814b901571cbbb6a2ac443afb777b299e67h",3.0f),
        Entity("037e139da7567349dc47cfa32fec5774556addfdf110fd8eedae09f9ca678b0033h",1.0f),
        Entity("03d393e113c107d6aa1b710623f65ddb6abe7179a8ab595ce7587ad97b43acf193h",3.0f) };


    Transaction tx4("2021-07-23 19:41:30", "037e139da7567349dc47cfa32fec5774556addfdf110fd8eedae09f9ca678b0033h", 1.5f);
    tx4.version = MY_CRYPTO_VERSION;
    tx4.inputs = { "477ED9817BCA3D870CF4FB06BA26951CFC865B6B4641D4C85BD8A30F006BFD6D" };
    tx4.outputs = { Entity("02d3df2c1aeedb13b9a29af0e3d42a4f19ac3187b49377c78fe9b7844c69bf1ea3h",298.5f),
        Entity("0390562eed62f9563117ffb2fa3b2e8814b901571cbbb6a2ac443afb777b299e67h",500.0f),
        Entity("037e139da7567349dc47cfa32fec5774556addfdf110fd8eedae09f9ca678b0033h",100.0f),
        Entity("03d393e113c107d6aa1b710623f65ddb6abe7179a8ab595ce7587ad97b43acf193h",100.0f) };


   

    
   
   
    // signin transaction
    tx1.signature = "76FA36E6703F547E67E87E1EED082DA0A596418E6727941DC55061B5467143C9E3902DD0D4814829B578C367B8A45C9D65675BB600DAF378EA66E7449422AA7C";
    tx2.signature = "67D5BCFFD8EC4466521CD158559718EE4A2847F8292A39AB1FF7BF8A4D5E7E147D0BA32EE3C794DDBE0C9AC0483F6DAEBF018115063D940F90378E43B6DD2417";
    tx3.signature = "D64C04A528A6D52384867BAE240CBFEBB5829BA103E4222B046CC1C9A3E2CD2813FE0321C81A3BF19D7293DCC178F9C4814F0229358E45839896B46ADB1283C7";
    tx4.signature = "077E906CC9F06BD47B4099AB61C99E2BB256B633792846F63F0EB73391BD1BD7509D9BC94634CE93BDD2150C52B848E02E6527D1B0AD386FACA853EFA545F4F2";

    //tx4.signature = node1.wallet.sign_tx(tx4.tx_to_json());

    tx1.compute_hash();
    tx2.compute_hash();
    tx3.compute_hash();
    tx4.compute_hash();

    
    /*std::cout << tx4.signature << std::endl;
    std::cout << tx4.hash << std::endl;*/

    std::cout << Tools::sign_verifier(tx1.origin, tx1.signature, tx1.tx_to_json()) << std::endl;
    std::cout << Tools::sign_verifier(tx2.origin, tx2.signature, tx2.tx_to_json()) << std::endl;
    std::cout << Tools::sign_verifier(tx3.origin, tx3.signature, tx3.tx_to_json()) << std::endl;
    std::cout << Tools::sign_verifier(tx4.origin, tx4.signature, tx4.tx_to_json()) << std::endl;


    
    std::cout << node1.validate_tx(tx1) << std::endl;
    std::cout << node1.validate_tx(tx2) << std::endl;
    std::cout << node1.validate_tx(tx3) << std::endl;
    std::cout << node1.validate_tx(tx4) << std::endl;
    
  


    Block block("02d3df2c1aeedb13b9a29af0e3d42a4f19ac3187b49377c78fe9b7844c69bf1ea3h");
    block.time = "2021-07-23 20:41:30";
    block.version = MY_CRYPTO_VERSION;
    block.ID = 1;
    block.father_hash = "000000000000000000000000000000000000000000000000";
    block.transaction_list = { tx1, tx2, tx3, tx4 };
    block.reward = block.compute_block_reward();

    block.find_mt_root();

    block.work_hash = Tools::hash_sha256(block.block_to_json());

    

    node1.blockchain.insert_block(block);

    Block blk_back;

    node1.blockchain.get_block(blk_back, 1);

    //std::cout << blk_back.block_to_json(true) << std::endl;


    
    node1.blockchain_head = node1.blockchain.get_head();
   

    std::cout << node1.validate_block(block) << std::endl;
   

    std::cout << "...ea3h = " << node1.blockchain.get_balance("02d3df2c1aeedb13b9a29af0e3d42a4f19ac3187b49377c78fe9b7844c69bf1ea3h") << std::endl;
    std::cout << "...e67h = " << node1.blockchain.get_balance("0390562eed62f9563117ffb2fa3b2e8814b901571cbbb6a2ac443afb777b299e67h") << std::endl;
    std::cout << "...033h = " << node1.blockchain.get_balance("037e139da7567349dc47cfa32fec5774556addfdf110fd8eedae09f9ca678b0033h") << std::endl;
    std::cout << "...193h = " << node1.blockchain.get_balance("03d393e113c107d6aa1b710623f65ddb6abe7179a8ab595ce7587ad97b43acf193h") << std::endl;
   

    //node1.validate_inputs({ "B5D5A456F11269CD696378E21B2901FE86346CA4FE0EDF22897BFCE86B9AC26E" , "ACB7B78CC31F5E8F78F1C38E2A907A2437D68A759AFC498972AE824FB27C8D7C"}, "037e139da7567349dc47cfa32fec5774556addfdf110fd8eedae09f9ca678b0033h");
   
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

