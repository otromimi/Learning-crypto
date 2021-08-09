#include <string>
#include <vector>
#include <chrono>

#include <iostream>
#include <sstream>

#include "struct_mapping.h"

struct Transaction {

    //Fields to be hash
    std::string version;
    std::vector<std::string> inputs;
    std::string destination;
    unsigned int value;
    std::string owner;
    unsigned int cashback;
    unsigned int fee;
    std::string tx_id;//hash
    std::string signature;

    
    // Overloading operators for <<
    friend std::ostream& operator << (std::ostream& outstream, Transaction& data) {
        outstream << "-----Transaction: " << data.tx_id << "-----\n" <<
            "Version: " + data.version << "\n" <<
            "Inputs: " << data.inputs.size() << "\n" <<
            "Destination: " << data.destination << "\n" <<
            "Value: " << data.value << "\n" <<
            "Owner: " << data.owner << "\n" <<
            "Cashback: " << data.cashback << "\n" <<
            "Fee: " << data.fee << "\n" <<
            "Signature: " << data.signature << "\n" <<
            "--------------------------------------\n";
           
        return outstream;
    }

    std::string to_string() {
        return "-----Transaction: " + this->tx_id + "-----\n" +
            "Version: " + this->version + "\n" +
            //"Inputs: " + this->inputs.size() + "\n" +
            "Destination: " + this->destination + "\n" +
            "Version: " + this->version + "\n" +
            "Version: " + this->version + "\n" +
            "Version: " + this->version + "\n" +
            "Version: " + this->version + "\n" +
            "Version: " + this->version + "\n" +
            "Version: " + this->version + "\n" +
            "Version: " + this->version + "\n" +
            "Version: " + this->version + "\n";
    }

    Transaction(){}

    Transaction(std::string version,
    std::string destination,
    unsigned int value,
    std::string owner,
    unsigned int cashback,
    unsigned int fee):
        version(version), destination(destination), value(value), owner(owner), cashback(cashback), fee(fee){}

    static std::string tx_to_json(Transaction tx){
        struct_mapping::reg(&Transaction::version, "Version");
        struct_mapping::reg(&Transaction::inputs, "Inputs");
        struct_mapping::reg(&Transaction::destination, "Destination");
        struct_mapping::reg(&Transaction::value, "Value");
        struct_mapping::reg(&Transaction::owner, "Owner");
        struct_mapping::reg(&Transaction::cashback, "Cashback");
        struct_mapping::reg(&Transaction::fee, "Fee");
        struct_mapping::reg(&Transaction::tx_id, "Tx_ID");
        
        std::ostringstream tx_json;
        struct_mapping::map_struct_to_json(tx, tx_json);

        std::cout << tx_json.str() << std::endl;

        return tx_json.str();
    }

};

struct Block {
    std::vector<Transaction> transaction_list;
    std::string father_hash;
    std::string work_hash;
    unsigned long nonce;
    std::chrono::time_point<std::chrono::system_clock> time;


};
