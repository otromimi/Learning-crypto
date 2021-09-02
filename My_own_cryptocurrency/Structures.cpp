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

Transaction::Transaction(): time(""), version("-.-.-"), origin(""), fee(0), hash(""), signature("") {}

Transaction::Transaction(std::string time,std::string version, std::string origin, float fee, std::string signature) :
    time(time), version(version), origin(origin), fee(fee), hash(""), signature(signature){
    /*
    std::time_t now = std::time(0);
    long peter = now;
    char* dt = ctime(&now);


    std::tm* utc_struct = std::gmtime(&now);
    char* time_UTC_string = std::asctime(utc_struct);
    *(time_UTC_string+24) = 0x00;*/

    

}


std::string Transaction::tx_to_json(bool indent) {
    //struct_mapping::detail::Reset::reg(struct_mapping::detail::Functions);

    

    struct_mapping::reg(&Entity::account, "Account");
    struct_mapping::reg(&Entity::value, "Value");

    struct_mapping::reg(&Transaction::version, "Version");
    struct_mapping::reg(&Transaction::time, "Time");
    struct_mapping::reg(&Transaction::inputs, "Inputs");
    struct_mapping::reg(&Transaction::outputs, "Outputs");
    struct_mapping::reg(&Transaction::origin, "Origin");
    struct_mapping::reg(&Transaction::fee, "Fee");
    struct_mapping::reg(&Transaction::signature, "Signature");


    std::ostringstream tx_json;
    if(indent)
        struct_mapping::map_struct_to_json(*this, tx_json, "  ");
    else
        struct_mapping::map_struct_to_json(*this, tx_json);


    return tx_json.str();
}

void Transaction::json_to_tx(std::string tx_json) {
   
    struct_mapping::reg(&Entity::account, "Account");
    struct_mapping::reg(&Entity::value, "Value");

    struct_mapping::reg(&Transaction::version, "Version");
    struct_mapping::reg(&Transaction::time, "Time");
    struct_mapping::reg(&Transaction::inputs, "Inputs");
    struct_mapping::reg(&Transaction::outputs, "Outputs");
    struct_mapping::reg(&Transaction::origin, "Origin");
    struct_mapping::reg(&Transaction::fee, "Fee");
    struct_mapping::reg(&Transaction::signature, "Signature");



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
    this->hash = Tools::hash_sha256(tx_to_json());
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
    struct_mapping::reg(&Block::reward, "Reward");
    struct_mapping::reg(&Block::mt_root, "MT_root");
    struct_mapping::reg(&Block::ID, "ID");
    struct_mapping::reg(&Block::nonce, "Nonce");
    struct_mapping::reg(&Block::work_hash, "Pow");
    struct_mapping::reg(&Block::transaction_list, "Transaction_list");
       
    std::ostringstream block_json;
    if (indent)
        struct_mapping::map_struct_to_json(*this, block_json, "  ");
    else
        struct_mapping::map_struct_to_json(*this, block_json);


    return block_json.str();
}

void Block::json_to_block(std::string blk_json) {

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
    struct_mapping::reg(&Block::reward, "Reward");
    struct_mapping::reg(&Block::mt_root, "MT_root");
    struct_mapping::reg(&Block::ID, "ID");
    struct_mapping::reg(&Block::nonce, "Nonce");
    struct_mapping::reg(&Block::work_hash, "Pow");
    struct_mapping::reg(&Block::transaction_list, "Transaction_list");

    std::istringstream jason_data(blk_json);

    struct_mapping::map_json_to_struct(*this, jason_data);

}

float Block::compute_block_reward() {
    float reward = 0;
    for (Transaction i : this->transaction_list)
        reward += i.fee;
    return reward;
}






/// Overloading << operator
std::ostream& operator << (std::ostream& outstream,Entity& data) {

    int vpos = 12;
    outstream << "\t" << "..." << data.account.substr(data.account.size() - vpos, vpos) << "\t" << data.value << "\n";

    return outstream;
}

/// Overloading << operator
std::ostream& operator << (std::ostream& outstream, Transaction& data) {
   
    int vpos = 12;
    if (data.hash == "") {
        data.compute_hash();
    }

    outstream << " -----------Transaction-----------\n" <<
        " Version: " << data.version << "\n" <<
        " Time: " << data.time << "  UTC" << "\n" <<
        " Inputs: { \n";
    for (std::string i : data.inputs) {
        outstream << "\t..." << i.substr(i.size() - vpos, vpos) << std::endl;
    }
    outstream << " }\n" <<
        " Outputs: { \n";
    for (Entity i : data.outputs) {
        outstream << "\t..." << i.account.substr(i.account.size() - vpos, vpos) << ", " << i.value << std::endl;;
    }
    outstream << " }\n" <<
        " Origin: " << "..." << data.origin.substr(data.origin.size() - vpos, vpos) << "\n" <<
        " Fee: " << data.fee << "\n" <<
        " Sign: " << "..." << data.signature.substr(data.signature.size() - vpos, vpos) << "\n" <<
        " Hash: " << "..." << data.hash.substr(data.hash.size() - vpos, vpos) << "\n" <<
        " ----------------------------------\n";

    return outstream;
}

/// Overloading << operator
std::ostream& operator << (std::ostream& outstream, Block& data) {
    int vpos = 12;
    if (data.transaction_list.size() != 0) {
        for (int i = 0; i < data.transaction_list.size(); i++) {
            if (data.transaction_list[i].hash == "")
                data.transaction_list[i].compute_hash();
        }
    }

    outstream << " -------------Block " << data.ID << "------------- \n" <<
        " Version: " << data.version << "\n" <<
        " Time: " << data.time << "  UTC" << "\n" <<
        " Tx: { \n";
    for (Transaction i : data.transaction_list) {
        outstream << "\t..." << i.hash.substr(i.hash.size() - vpos, vpos) << std::endl;
    }
    outstream << " }\n" <<
        " Mt_root: " << "..." << data.mt_root.substr(data.mt_root.size() - vpos, vpos) << "\n" <<
        " Father: " << "..." << data.father_hash.substr(data.father_hash.size() - vpos, vpos) << "\n" <<
        " Reward: " << data.reward << "\n" <<
        " Miner: " << "..." << data.miner.substr(data.miner.size() - vpos, vpos) << "\n" <<
        " Nonce: " << data.nonce << "\n" <<
        " PoW: " << "..." << data.work_hash.substr(data.work_hash.size() - vpos, vpos) << "\n" <<
        " ----------------------------------\n";

    return outstream;
}


