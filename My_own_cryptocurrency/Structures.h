#pragma once


// Standard dependencies
#include <string>
#include <vector>
#include <ios>




// My own dependencies
#include "MerkleTree_sha256.h"

// Local dependencies
//#include "hex.h"


#define MAX_TX_IN_BLOCK 4
#define MAX_OUT_TX 12

namespace My_own_crypto {


    /// <summary>
    /// Structure used for the outputs of a transaction.
    /// </summary>
    struct Entity {
        std::string account; // Public key of the receiver
        float value;

        Entity() {};

        Entity(std::string account, float value);

        std::string to_json(bool indent, bool full);

       
    };

    /// <summary>
    /// Transaction.
    /// </summary>
    struct Transaction {
    public:

        // Fields to be hash
        std::string time; // Time stamp of the transaction
        std::string version;
        std::vector<Entity> outputs;
        std::vector<std::string> inputs;
        std::string origin; // Transation owner PK
        float fee;

        // No sign
        std::string hash;
        std::string signature;



        /// <summary>
        /// Plain constructor
        /// </summary>
        Transaction();


        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="origin"></param>
        /// <param name="value"></param>
        /// <param name="fee"></param>
        /// <param name="signature"></param>
        Transaction(std::string time, std::string = "-.-.-", std::string origin = "", float fee = 0, std::string = "");

        /// <summary>
        /// Return a JSON format string of the transaction.
        /// UPDATE Second argument is dummy.
        /// </summary>
        /// <param name="indent">True if you what it to be nice formated, False if you want it all in one string.</param>
        /// <param name="full">True if you want to include the signature on the JSON, False otherwise.</param>
        /// <returns>JSON formated transaction</returns>
        std::string tx_to_json(bool = false);

        /// <summary>
        /// Populates the transaction object with the passed JSON data.
        /// UPDATE Second argument is dummy.
        /// </summary>
        /// <param name="tx_json">JSON data in string format.</param>
        /// <param name="full">True the JSON includes the signature, False otherwise.</param>
        void json_to_tx(std::string tx_json);

        /// <summary>
        /// Returs the string used for DB operations.
        /// </summary>
        /// <returns>""value1", "value2", "value3""</returns>
        std::string to_db_string();

        /// <summary>
        /// Computes hash of the Tx and returns it.
        /// </summary>
        std::string compute_hash();


    };

    struct Block {

        // Proof of work fields
        std::string father_hash;
        std::string version;
        std::string time;
        std::string miner;
        float reward;
        std::string mt_root; //merkle tree root
        unsigned int ID;
        unsigned int nonce;

        // Non hased fields
        std::string work_hash;
        std::vector<Transaction> transaction_list;
        MerkleTree mt; // From here only hash root.

        Block();

        Block(std::string miner);

        /// <summary>
        /// Creates the hash tree with the transactions list.
        /// </summary>
        void find_mt_root();


        /// <summary>
        /// Block structure to JSON string.
        /// UPDATE Second argument is dummy.
        /// </summary>
        /// <param name="indent">Pretty format.</param>
        /// <param name="full">This parameter is dummy, (doesn't work).</param>
        /// <returns>String with the block data to be hashed.</returns>
        std::string block_to_json(bool = false, bool = false);

        void json_to_block(std::string blk_json);

        /// <summary>
        /// Populates the field reward int he block.
        /// </summary>
        /// <returns>Sum of transactions fees.</returns>
        float compute_block_reward();
    };


}


/// Overloading << operator
std::ostream& operator << (std::ostream& outstream, My_own_crypto::Entity& datas);

/// Overloading << operator
std::ostream& operator << (std::ostream& outstream, My_own_crypto::Transaction& data);

/// Overloading << operator
std::ostream& operator << (std::ostream& outstream, My_own_crypto::Block& data);