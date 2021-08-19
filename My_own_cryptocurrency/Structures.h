#pragma once


// Standard dependencies
#include <string>
#include <vector>
#include <ctime>



// My own dependencies


// Local dependencies
#include "hex.h"
#include "struct_mapping.h"



namespace My_own_crypto {


    /// <summary>
    /// Structure used for the outputs of a transaction.
    /// </summary>
    struct Entity {
        std::string account; // Public key of the receiver
        float value;

       

        Entity(std::string account, float value);

       
    };

    /// <summary>
    /// Transaction.
    /// </summary>
    struct Transaction {

        // Fields to be hash
        long time; // Time stamp of the transaction
        std::vector<std::string> inputs;
        std::vector<Entity> outputs;
        std::string origin; // Transation owner PK
        float value; // Cumulative value of inputs///  No 100%
        float fee;
        std::string signature;
        std::string hash;


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
        Transaction(std::string origin, float value, float fee);

        /// <summary>
        /// Return a JSON format string of the transaction.
        /// </summary>
        /// <param name="indent">True if you what it to be nice formated, False if you want it all in one string.</param>
        /// <returns>JSON formated transaction</returns>
        std::string tx_to_json(bool indent);

        /// <summary>
        /// Populates the transaction object with the passed JSON data.
        /// </summary>
        /// <param name="tx_json">JSON data in string format.</param>
        void json_to_tx(std::string tx_json);

        /// <summary>
        /// Returs the string used for DB operations.
        /// </summary>
        /// <returns>""value1", "value2", "value3""</returns>
        std::string to_db_string();

        

        

    };

    

    struct Block {

        // Proof of work fields
        std::string father_hash;
        std::string time;
        std::string miner;
        float reward;
        std::vector<Transaction> transaction_list;
        unsigned int ID;
        unsigned int nonce;
        // Non hased fields
        std::string work_hash;


    };


    
   

}


/// Overloading << operator
std::ostream& operator << (std::ostream& outstream, My_own_crypto::Entity& datas);

/// Overloading << operator
std::ostream& operator << (std::ostream& outstream, My_own_crypto::Transaction& data);