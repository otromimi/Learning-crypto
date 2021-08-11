#pragma once

// Standard dependencies
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <ctime>



// My own dependencies


// Local dependencies
#include "struct_mapping.h"

struct Entity {
    std::string account; // Public key of the receiver
    unsigned int value;

    Entity(std::string account, unsigned int value) :account(account), value(value) {}

    // Overloading operators for <<
    friend std::ostream& operator << (std::ostream& outstream, Entity& data) {
        outstream << "\t" << data.account << "\t" << data.value << "\n";

        return outstream;
    }
};

struct Transaction {

    //Fields to be hash
    std::string version;
    std::string time;
    std::vector<std::string> inputs;
    std::vector<Entity> outputs;
    std::string origin;
    unsigned int fee;
    unsigned int value;
    std::string signature;

    
    // Overloading operators for <<
    friend std::ostream& operator << (std::ostream& outstream, Transaction& data) {
        outstream << "-----Transaction: " << "-----\n" <<
            " UTC " << data.time << "\n" <<
            "Version: " << data.version << "\n" <<
            "Inputs: { ";
            for (std::string i : data.inputs) {
                outstream << i << ", ";
            }
            outstream << "\b\b }\n" <<
            "Outputs: \n";
            for (Entity i : data.outputs) {
                outstream << i;
            }
            outstream << "\n" <<
            "Origin: " << data.origin << "\n" <<
            "Value: " << data.value << "\n" <<
            "Fee: " << data.fee << "\n" <<
            "\n --Sign-- \n" << data.signature << "\n" <<
            "--------------------------------------\n";
           
        return outstream;
    }

   
    Transaction(){}

    Transaction(std::string version, unsigned int value, std::string origin, unsigned int fee):
        version(version), value(value), origin(origin), fee(fee){

        std::time_t now = std::time(0);
        char* dt = ctime(&now);
        //std::cout << dt << std::endl;

        std::tm* utc_struct = std::gmtime(&now);
        char* time_UTC_string = std::asctime(utc_struct);

        this->time = time_UTC_string;

        //std::cout << time_UTC_string << std::endl;
    }

    std::string tx_to_json() {
        struct_mapping::reg(&Transaction::version, "Version");
        struct_mapping::reg(&Transaction::inputs, "Inputs");
        struct_mapping::reg(&Transaction::outputs, "Outputs");
        struct_mapping::reg(&Transaction::value, "Value");
        struct_mapping::reg(&Transaction::origin, "Origin");
        struct_mapping::reg(&Transaction::fee, "Fee");
        
        
        std::ostringstream tx_json;
        struct_mapping::map_struct_to_json(*this, tx_json);

        //std::cout << tx_json.str() << std::endl;

        return tx_json.str();
    }

    void json_to_tx(std::string tx_json) {
        struct_mapping::reg(&Transaction::version, "Version");
        struct_mapping::reg(&Transaction::inputs, "Inputs");
        struct_mapping::reg(&Transaction::outputs, "Outputs");
        struct_mapping::reg(&Transaction::value, "Value");
        struct_mapping::reg(&Transaction::origin, "Origin");
        struct_mapping::reg(&Transaction::fee, "Fee");

        //  std::ostringstream tx_json;

        std::istringstream jason_data(tx_json);

        struct_mapping::map_json_to_struct(*this, jason_data);

        //std::cout << tx_json.str() << std::endl;

    }

};


struct Block {
    
    std::string father_hash;
    std::string time;
    std::string miner;
    unsigned int reward;
    std::vector<Transaction> transaction_list;
    unsigned int ID;
    unsigned int nonce;
    std::string work_hash;
    

};
