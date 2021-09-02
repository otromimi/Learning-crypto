#pragma once

#include "sqlite3.h"
#include "Structures.h"

namespace My_own_crypto {

    enum Element{
        BLOCKS,
        TRANSACTIONS,
        OUTPUTS,
        INPUTS
    };

    class DB_operations {

        sqlite3* db;
        mutable char* zErrMsg = 0;
        const char* data;

        /// <summary>
        /// Fuctions called after quering the database.
        /// </summary>
        static int callback(void* NotUsed, int argc, char** argv, char** azColName);

        static int parse_block(void* blk, int argc, char** argv, char** azColName);

        static int callback_tx_count(void* tx_list, int argc, char** argv, char** azColName);

        static int parse_transactions(void* tx_list, int argc, char** argv, char** azColName);

        static int parse_outputs(void* tx, int argc, char** argv, char** azColName);

        static int parse_inputs(void* tx, int argc, char** argv, char** azColName);

        static int callback_balance(void* balance, int argc, char** argv, char** azColName);

        static int parse_transaction(void* tx, int argc, char** argv, char** azColName);

        static int callback_head(void* head, int argc, char** argv, char** azColName);

        static int callback_inputs(void* inputs_full, int argc, char** argv, char** azColName);

        static int callback_unespent_tx(void* utx_v, int argc, char** argv, char** azColName);



        /// <summary>
        /// Creating tables for the database
        /// </summary>
        void create_tables();


    public:

        /// <summary>
        /// Opening database
        /// </summary>
        DB_operations(std::string db_name);

        /// <summary>
        /// Inserting block into the database.
        /// </summary>
        /// <param name="table">Table where the data is gonna be inserted.</param>
        /// <param name="data">List of parameter to be inserted in each row.</param>
        int insert(Element table, std::string data) const;

        /// <summary>
        /// Inserting block into the database.
        /// </summary>
        /// <param name="table">Table where the data is gonna be inserted.</param>
        /// <param name="data">List of parameter to be inserted in each row.</param>
        void insert_block(Block& block) const;


        void get_tx_count(std::vector<std::string>& tx_list, std::string hash);

        void get_tx(Transaction& tx, std::string hash);

        /// <summary>
        /// Populates the reference block with the data from the DB, with the desire block ID.
        /// </summary>
        /// <param name="blk">Block to populate.</param>
        /// <param name="block_id">ID of block to be retrieve.</param>
        void get_block(Block& blk, int block_id);

        /// <summary>
        /// Check the balane of an account.
        /// </summary>
        /// <param name="account">Account to check the balance.</param>
        /// <returns>Balance</returns>
        float get_balance(std::string account);


        void get_inputs(std::vector<Entity>& inputs_full, std::string address, std::string time);

        /// <summary>
        /// Populates the vector<Entity> with the unespent outputs.
        /// </summary>
        /// <param name="utx_v">Vector to populate with utx.</param>
        /// <param name="address">Retrieve utx from this address.</param>
        /// <param name="time">Retrieve only utx older than this time.</param>
        void get_unespent_tx(std::vector<Entity>& utx_v, std::string address, std::string time);

        /// <summary>
        /// Select values from database
        /// </summary>
        std::string select(Element table, std::string data);


        /// <summary>
        /// Gets the head of the blockchain.
        /// </summary>
        /// /// <returns>Last block ID.</returns>
        unsigned int get_head();


        void print_select(Element table, std::string data);

        /// <summary>
        /// Destroying instance and closing database.
        /// </summary>
        /// <param name="table"></param>
        /// <param name="data"></param>
        ~DB_operations();
    };

}