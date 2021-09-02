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
#include <future>
#include <thread>
#include <mutex>

// My own dependencies

#include "Node.h"
#include "Network.h"


using namespace My_own_crypto;

#define MY_CRYPTO_VERSION "1.0.0"

// Below fuctions declarations
int menu();
void press2cont();

/// first argument: server port
/// second argument: client connection port
int main(int argc, char* argv[])
{

    
    Node node1("User3", MY_CRYPTO_VERSION);
   
 
    // First transaction (primigine transaciton), the one that puts founds on the different accounts.
    Transaction tx1("2021-07-23 18:40:25", MY_CRYPTO_VERSION, "02d3df2c1aeedb13b9a29af0e3d42a4f19ac3187b49377c78fe9b7844c69bf1ea3h", 0.0f);
  
    tx1.inputs = { "0000000000000000000000000000000000000000000000000000000000000000" };
    tx1.outputs = {Entity("02d3df2c1aeedb13b9a29af0e3d42a4f19ac3187b49377c78fe9b7844c69bf1ea3h",1000.0f), 
        Entity("0390562eed62f9563117ffb2fa3b2e8814b901571cbbb6a2ac443afb777b299e67h",1000.0f), 
        Entity("037e139da7567349dc47cfa32fec5774556addfdf110fd8eedae09f9ca678b0033h",1000.0f), 
        Entity("03d393e113c107d6aa1b710623f65ddb6abe7179a8ab595ce7587ad97b43acf193h",1000.0f) };

    Transaction tx2("2021-07-23 18:40:30", MY_CRYPTO_VERSION, "0390562eed62f9563117ffb2fa3b2e8814b901571cbbb6a2ac443afb777b299e67h", 2.f);

    tx2.inputs = { "F5C7D92D7017BF52DB11302DD2E9CE24DE86D6F475C17F1C7C261D3C6041F04F" };
    tx2.outputs = { Entity("02d3df2c1aeedb13b9a29af0e3d42a4f19ac3187b49377c78fe9b7844c69bf1ea3h",100.0f),
         Entity("0390562eed62f9563117ffb2fa3b2e8814b901571cbbb6a2ac443afb777b299e67h",698.0f),
         Entity("037e139da7567349dc47cfa32fec5774556addfdf110fd8eedae09f9ca678b0033h",100.0f),
         Entity("03d393e113c107d6aa1b710623f65ddb6abe7179a8ab595ce7587ad97b43acf193h",100.0f) };

    Transaction tx3("2021-07-23 18:41:30", MY_CRYPTO_VERSION,  "037e139da7567349dc47cfa32fec5774556addfdf110fd8eedae09f9ca678b0033h", 10.f);

    tx3.inputs = { "74075039CB00DB764CD2385E8203F2BA09B5934072E526E86C87E8946596AE47"};
    tx3.outputs = { Entity("02d3df2c1aeedb13b9a29af0e3d42a4f19ac3187b49377c78fe9b7844c69bf1ea3h",20.0f),
        Entity("0390562eed62f9563117ffb2fa3b2e8814b901571cbbb6a2ac443afb777b299e67h",30.0f),
        Entity("037e139da7567349dc47cfa32fec5774556addfdf110fd8eedae09f9ca678b0033h",10.0f),
        Entity("03d393e113c107d6aa1b710623f65ddb6abe7179a8ab595ce7587ad97b43acf193h",30.0f) };


    Transaction tx4("2021-07-23 19:41:30", MY_CRYPTO_VERSION, "037e139da7567349dc47cfa32fec5774556addfdf110fd8eedae09f9ca678b0033h", 1.5f);
 
    tx4.inputs = { "F5C7D92D7017BF52DB11302DD2E9CE24DE86D6F475C17F1C7C261D3C6041F04F" };
    tx4.outputs = { Entity("02d3df2c1aeedb13b9a29af0e3d42a4f19ac3187b49377c78fe9b7844c69bf1ea3h",298.5f),
        Entity("0390562eed62f9563117ffb2fa3b2e8814b901571cbbb6a2ac443afb777b299e67h",500.0f),
        Entity("037e139da7567349dc47cfa32fec5774556addfdf110fd8eedae09f9ca678b0033h",100.0f),
        Entity("03d393e113c107d6aa1b710623f65ddb6abe7179a8ab595ce7587ad97b43acf193h",100.0f) };
   
    // signin transaction
    tx1.signature = "37EFD26CA810CB33412DC04EDE9A2DE58EA8D1768371982018806C2DC4B406B3CF32D2610C791C084464F415B378F8DE9AF12C14915B7C5E6CA1C08284BD6414";
    tx2.signature = "524E56347A0114411F0F3C01B462DF47BC21E1EA84E7AE4958EF6A1DB6F8D34C7FC94968D048BA4472135D949BB15D0B09F4687CBD42CC9AA673EB0FD02DB4CB";
    tx3.signature = "F68F87768BF4CC1EE3556FBB2B140A4C0A69E2A0959C1338515470169E97FE2F577697A2174EC33CBAA9FC49EE4ED19B0E2AE7C4EA46F19541BA4FE676300FD8";
    tx4.signature = "837D310D377EFFFD59A3C3E1FD8579C96A6F8D0A33790808ED9B27CCB8C20CB11663F650A27843DC99A3387621657B316BAB6345A1558EFD3483033F0B5BEC03";



    
    /*tx1.signature = node1.wallet.sign_tx(tx1.tx_to_json());
    tx2.signature = node1.wallet.sign_tx(tx2.tx_to_json());
    tx3.signature = node1.wallet.sign_tx(tx3.tx_to_json());
    tx4.signature = node1.wallet.sign_tx(tx4.tx_to_json());*/

    tx1.compute_hash();
    tx2.compute_hash();
    tx3.compute_hash();
    tx4.compute_hash();

    
    /*std::cout << tx4.signature << std::endl;
    std::cout << tx4.hash << std::endl;*/

    

    std::cout << node1.validate_sign(tx1) << std::endl;
    std::cout << node1.validate_sign(tx2) << std::endl;
    std::cout << node1.validate_sign(tx3) << std::endl;
    std::cout << node1.validate_sign(tx4) << std::endl;


    
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
    //bool checking = Tools::sign_verifier(node1.wallet.get_compressedPublic(),tx.signature, tx.tx_to_json(false, false));

    

    //blockchain_db.insert(Element::TRANSACTION, tx.to_db_string());
    //blockchain_db.select(Element::TRANSACTION, tx1.to_db_string());

    //Transaction tx = node1.create_tx();

    ///////////////////Networking ///////////////////////////////////

    /*std::vector<std::string> names;
    Transaction jj = tx1;*/

    extern std::mutex server_mutex;
    extern std::mutex client_mutex;
    extern std::atomic<bool> server_running;

    
    
    
    // Running server in a separate thread
    std::thread test(&runServer, std::ref(node1.blockchain), std::ref(node1.blockchain_head), std::ref(node1.confirmed_tansactions), "5762");
    test.detach();

    //server_running = true;
    //node1.run_server("5762");
    node1.confirmed_tansactions.push_back(tx1);
    node1.confirmed_tansactions.push_back(tx2);

    Transaction tx;

    int selected_option = 0;
    do {
        selected_option = menu();
        switch (selected_option)
        {
        case 1:
            //create transaction
            node1.create_tx();
            press2cont();
            break;
        case 2:
            //mine block
            break;
        case 3:
            //sync node
            break;
        case 4:
            //print block
            node1.print_block();
            press2cont();
            break;
        case 5:
            //print transaction
            node1.print_transaction();
            break;
        case 6:
            //wallet info
            node1.print_wallet_info();
            press2cont();
            break;
        case 0:
            //write and save
            node1.check_node("5773");
            break;
        default:
            break;
        }
    } while (selected_option != 0);
    
   


    //Server::mutex.lock();
    //for (int i = 0; i < 102; i++)
    //    std::cout << "&";
    //Server::mutex.unlock();

    

    //server_running = false;
    //node1.~Node();
    

    //test.join();
  

    //if (_DEBUG) { //braaks in linux
    // Stoping execution in linux
    if (PLATFORM_NAME != "windows") {
        std::cout << "Platform: " << PLATFORM_NAME << std::endl;
        std::cin.get();
    }
    //}

    return 0;
}

int menu() {
    std::string str_selection;
    int selection = -1;

    do {

        std::cout << "\x1B[2J\x1B[H";// clear screen

        std::cout << "\n=============My_own_cryptocurrency=============\n" <<
            "\n\tSelect one of the options: \n" <<
            "\t 1.- Create transaction\n" <<
            "\t 2.- Mine block\n" <<
            "\t 3.- Sync node\n" <<
            "\t 4.- Print block\n" <<
            "\t 5.- Print transaction\n" <<
            "\t 6.- Wallet info\n" <<
            "\t 0.- Write and save\n" <<
            "\n\t >>> ";

        std::cin >> str_selection;
        try {
            selection = std::stoi(str_selection);
        }
        catch (std::exception e) {
            selection = -1;
            std::cin.clear();
        }
    } while (selection == -1);

    //std::cout << str_selection << std::endl;

    std::cout << "\x1B[2J\x1B[H";// clear screen

    return selection;
}

void press2cont() {
    std::cout << "Press enter to continue...";
    std::cin.clear();
    std::string aux;
    std::getline(std::cin, aux);
    std::cin.get();
    std::cin.clear();
}