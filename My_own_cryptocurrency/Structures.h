#pragma once


// Standard dependencies
#include <string>
#include <vector>
#include <ctime>



// My own dependencies


// Local dependencies
#include "hex.h"
#include "struct_mapping.h"







struct Entity {
    std::string account; // Public key of the receiver
    unsigned int value;

    Entity(std::string account, unsigned int value);


};


struct Transaction {

    // Fields to be hash
    long time;
    std::vector<std::string> inputs;
    std::vector<Entity> outputs;
    std::string origin; // Transation owner PK
    unsigned int value;
    unsigned int fee;
    std::string signature;



    Transaction();

    Transaction(std::string origin, unsigned int value, unsigned int fee);


    std::string tx_to_json(bool indent);

    void json_to_tx(std::string tx_json);

    std::string to_db_string();

};



struct Block {

    // Proof of work fields
    std::string father_hash;
    std::string time;
    std::string miner;
    unsigned int reward;
    std::vector<Transaction> transaction_list;
    unsigned int ID;
    unsigned int nonce;
    // Non hased fields
    std::string work_hash;


};



/// Overloading << operator
std::ostream& operator << (std::ostream& outstream, Entity& data);

/// Overloading << operator
std::ostream& operator << (std::ostream& outstream, Transaction& data);