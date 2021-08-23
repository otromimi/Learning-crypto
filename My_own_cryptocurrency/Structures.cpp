#include <iostream>
#include <sstream>

#include "Structures.h"
#include "struct_mapping.h"
#include "Tools.h"

using namespace My_own_crypto;


///////////////////////////////////// Entity /////////////////////////////////////

Entity::Entity(std::string account, float value) :account(account), value(value) {}

std::string Entity::to_json(bool indent, bool full) {

    struct_mapping::reg(&Entity::account, "Account");
    struct_mapping::reg(&Entity::value, "Value");

    std::ostringstream tx_json;
    if (indent)
        struct_mapping::map_struct_to_json(*this, tx_json, "  ");
    else
        struct_mapping::map_struct_to_json(*this, tx_json);

    return tx_json.str();
}


///////////////////////////////////// Transaction /////////////////////////////////////

Transaction::Transaction(std::string time, std::string origin, float fee, std::string signature) :
    time(time), origin(origin), fee(fee), signature(signature){
    /*
    std::time_t now = std::time(0);
    long peter = now;
    char* dt = ctime(&now);


    std::tm* utc_struct = std::gmtime(&now);
    char* time_UTC_string = std::asctime(utc_struct);
    *(time_UTC_string+24) = 0x00;*/

    

}


std::string Transaction::tx_to_json(bool indent, bool full) {


    struct_mapping::reg(&Entity::account, "Account");
    struct_mapping::reg(&Entity::value, "Value");

    struct_mapping::reg(&Transaction::version, "Version");
    struct_mapping::reg(&Transaction::time, "Time");
    struct_mapping::reg(&Transaction::inputs, "Inputs");
    struct_mapping::reg(&Transaction::outputs, "Outputs");
    struct_mapping::reg(&Transaction::origin, "Origin");
    struct_mapping::reg(&Transaction::fee, "Fee");
    /*if (full)
        struct_mapping::reg(&Transaction::signature, "Signature");*/

    std::ostringstream tx_json;
    if(indent)
        struct_mapping::map_struct_to_json(*this, tx_json, "  ");
    else
        struct_mapping::map_struct_to_json(*this, tx_json);


    return tx_json.str();
}

void Transaction::json_to_tx(std::string tx_json, bool full) {
   
    struct_mapping::reg(&Entity::account, "Account");
    struct_mapping::reg(&Entity::value, "Value");

    struct_mapping::reg(&Transaction::version, "Version");
    struct_mapping::reg(&Transaction::time, "Time");
    struct_mapping::reg(&Transaction::inputs, "Inputs");
    struct_mapping::reg(&Transaction::outputs, "Outputs");
    struct_mapping::reg(&Transaction::origin, "Origin");
    struct_mapping::reg(&Transaction::fee, "Fee");
    //if (full)
        //struct_mapping::reg(&Transaction::signature, "Signature");



    //  std::ostringstream tx_json;

    std::istringstream jason_data(tx_json);

    struct_mapping::map_json_to_struct(*this, jason_data);

    //std::cout << tx_json.str() << std::endl;

}

std::string Transaction::to_db_string() {
    std::ostringstream ordered_data;
    ordered_data << "\"" << this->signature << "\"" << ", " << 123 << ", " << this->time << ", \"" << this->origin << "\"," << this->fee;
    return ordered_data.str();
}


std::string Transaction::compute_hash() {
    this->hash = Tools::hash_sha256(tx_to_json(false, true));
    return this->hash;
}

///////////////////////////////////// Block /////////////////////////////////////



Block::Block(std::string miner) :miner(miner){
    this->time = Tools::time_now();
};


void Block::find_mt_root() {
    std::vector<std::string> hash_vector;

    for (Transaction i : this->transaction_list) {
        hash_vector.push_back(i.compute_hash());
    }
   
    this->mt.populateTree(hash_vector);
    this->mt_root = this->mt.root->hash;
    //this->mt.printTree(this->mt.root,0);
}

std::string Block::block_to_json(bool indent, bool full) {

    /*if (full) {
        struct_mapping::reg(&Entity::account, "Account");
        struct_mapping::reg(&Entity::value, "Value");

        struct_mapping::reg(&Transaction::version, "Version");
        struct_mapping::reg(&Transaction::time, "Time");
        struct_mapping::reg(&Transaction::inputs, "Inputs");
        struct_mapping::reg(&Transaction::outputs, "Outputs");
        struct_mapping::reg(&Transaction::origin, "Origin");
        struct_mapping::reg(&Transaction::fee, "Fee");
        struct_mapping::reg(&Transaction::signature, "Signature");
    }*/
    struct_mapping::reg(&Block::version, "Version");
    struct_mapping::reg(&Block::father_hash, "Father_hash");
    struct_mapping::reg(&Block::time, "Time");
    struct_mapping::reg(&Block::miner, "Miner");
    struct_mapping::reg(&Block::reward, "Time");
    struct_mapping::reg(&Block::mt_root, "MT_root");
    struct_mapping::reg(&Block::ID, "ID");
    struct_mapping::reg(&Block::ID, "Nonce");
    /*if (full) 
        struct_mapping::reg(&Block::work_hash, "Pow");
        struct_mapping::reg(&Block::transaction_list, "Transaction_list");*/
       
    std::ostringstream tx_json;
    if (indent)
        struct_mapping::map_struct_to_json(*this, tx_json, "  ");
    else
        struct_mapping::map_struct_to_json(*this, tx_json);


    return tx_json.str();
}

float Block::compute_block_reward() {
    float reward = 0;
    for (Transaction i : this->transaction_list)
        reward += i.fee;
    return reward;
}






/// Overloading << operator
std::ostream& operator << (std::ostream& outstream,Entity& data) {
    outstream << "\t" << data.account << "\t" << data.value << "\n";

    return outstream;
}

/// Overloading << operator
std::ostream& operator << (std::ostream& outstream, Transaction& data) {

    /*
    // Passing from epoch to UTC
    std::tm* utc_struct = std::gmtime((const time_t*)data.time);
    char* time_UTC_string = std::asctime(utc_struct);
    *(time_UTC_string + 24) = 0x00;
    */
    outstream << "-----Transaction: " << "-----\n" <<
        " UTC " << data.time << "\n" <<
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
        "Fee: " << data.fee << "\n" <<
        "\n --Sign-- \n" << data.signature << "\n" <<
        "--------------------------------------\n";

    return outstream;
}


