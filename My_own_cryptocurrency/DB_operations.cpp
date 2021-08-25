// Standard dependencies
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

// My own dependencies
#include "DB_operations.h"
#include "Structures.h"

using namespace My_own_crypto;

/// Queries

const char* select_tx = "SELECT * FROM TRANSACTIONS;";

const char* inputs_table = "CREATE TABLE INPUTS("  \
"ID             INTEGER PRIMARY KEY AUTOINCREMENT," \
"TX_HASH        TEXT                     NOT NULL," \
"INPUT          TEXT                     NOT NULL," \
"FOREIGN KEY (TX_HASH) REFERENCES TRANSACTIONS(HASH)" \
"ON DELETE CASCADE ON UPDATE CASCADE); ";

const char* outputs_table = "CREATE TABLE OUTPUTS("  \
"ID             INTEGER PRIMARY KEY AUTOINCREMENT," \
"TX_HASH        TEXT                     NOT NULL," \
"ACCOUNT        TEXT                     NOT NULL," \
"VALUE          REAL                     NOT NULL," \
"FOREIGN KEY (TX_HASH) REFERENCES TRANSACTIONS(HASH)" \
"ON DELETE CASCADE ON UPDATE CASCADE); ";

const char* transactions_table = "CREATE TABLE TRANSACTIONS("  \
"ID             INTEGER PRIMARY KEY AUTOINCREMENT," \
"HASH           TEXT                     NOT NULL," \
"VERSION        TEXT                     NOT NULL," \
"BLOCK          INT                      NOT NULL," \
"TIME           DATETIME                 NOT NULL," \
"OWNER          TEXT                     NOT NULL," \
"FEE            REAL                     NOT NULL," \
"SIGNATURE      TEXT                     NOT NULL," \
"FOREIGN KEY (BLOCK) REFERENCES BLOCKS(BLOCK_ID)" \
"ON DELETE CASCADE ON UPDATE CASCADE); ";

const char* blocks_table = "CREATE TABLE BLOCKS("  \
"BLOCK_ID       INT PRIMARY KEY          NOT NULL," \
"VERSION        TEXT                     NOT NULL," \
"TIME           DATETIME                 NOT NULL," \
"FATHER_HASH    TEXT                     NOT NULL," \
"WORK_HASH      TEXT                     NOT NULL," \
"MINER          TEXT                     NOT NULL," \
"REWARD         REAL                     NOT NULL," \
"MT_ROOT        TEXT                     NOT NULL," \
"NONCE          INT                      NOT NULL);";



int DB_operations::callback(void* NotUsed, int argc, char** argv, char** azColName) {
    int i;
    for (i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}


void DB_operations::create_tables() {

    int rc = sqlite3_exec(db, (const char*)blocks_table, callback, (void*)data, &zErrMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL log: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }

    rc = sqlite3_exec(db, (const char*)transactions_table, callback, 0, &zErrMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL log: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }

    rc = sqlite3_exec(db, (const char*)outputs_table, callback, 0, &zErrMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL log: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }

    rc = sqlite3_exec(db, (const char*)inputs_table, callback, 0, &zErrMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL log: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }
}



DB_operations::DB_operations(std::string db_name) {

    int rc = sqlite3_open((db_name + ".db").c_str(), &db);

    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
    }

    create_tables();
}



int DB_operations::insert(Element table, std::string data) const {

    std::string query = "INSERT INTO ";

    if (table == Element::INPUTS) {
        query += "INPUTS (TX_HASH, INPUT) VALUES (" + data + "); ";
    }
    if (table == Element::OUTPUTS) {
        query += "OUTPUTS (TX_HASH, ACCOUNT, VALUE) VALUES (" + data + "); ";
    }
    else if (table == Element::TRANSACTIONS) {
        query += "TRANSACTIONS (HASH, VERSION, BLOCK, TIME, OWNER, FEE,  SIGNATURE) VALUES (" + data + "); ";
    }
    else if (table == Element::BLOCKS) {
        query += "BLOCKS (BLOCK_ID, VERSION, TIME, FATHER_HASH, WORK_HASH, MINER, REWARD, MT_ROOT, NONCE) VALUES (" + data + "); ";
    }

    int rc = sqlite3_exec(db, (const char*)query.c_str(), callback, 0, &zErrMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }

    return rc;
}


void DB_operations::insert_block(Block& block) const {

    std::ostringstream blk_sink;
    std::ostringstream tx_sink;
    std::ostringstream output_sink;
    std::ostringstream input_sink;
    int rc;
    std::string tx_hash;

    blk_sink << block.ID << ", \"" << block.version << "\", \"" << block.time << "\", \"" << block.father_hash << "\", \"" << block.work_hash << "\", \""
        << block.miner << "\", " << block.reward << ", \"" << block.mt_root << "\", " << block.nonce;

    rc = insert(Element::BLOCKS, blk_sink.str());

    if (rc)
        return;

    for (Transaction tx : block.transaction_list) {
        tx_hash = tx.compute_hash();
        tx_sink << "\"" << tx_hash << "\", \"" << tx.version << "\", " << block.ID << ", \"" << tx.time << "\", \"" << tx.origin << "\", "
            << tx.fee << ", \"" << tx.signature << "\"";

        insert(Element::TRANSACTIONS, tx_sink.str());

        for (Entity out : tx.outputs) {
            output_sink << "\"" << tx_hash << "\", \"" << out.account << "\", " << out.value;
            
            insert(Element::OUTPUTS, output_sink.str());
            output_sink.str("");
        }

        for (std::string input : tx.inputs) {
            input_sink << "\"" << tx_hash << "\", \"" << input << "\"";

            insert(Element::INPUTS, input_sink.str());
            input_sink.str("");
        }
        tx_sink.str("");
    }
}

void DB_operations::get_tx(Transaction& tx, std::string hash) {
    std::string query = "SELECT HASH, VERSION, TIME, OWNER, FEE, SIGNATURE FROM TRANSACTIONS WHERE HASH='" + hash + "';";

    int rc = sqlite3_exec(db, (const char*)query.c_str(), parse_transaction, &tx, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }

    query = "SELECT ACCOUNT, VALUE FROM OUTPUTS WHERE TX_HASH='" + hash + "' ORDER BY ID ASC;";
    rc = sqlite3_exec(db, (const char*)query.c_str(), parse_outputs, &tx, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }

    query = "SELECT INPUT FROM INPUTS WHERE TX_HASH='" + hash + "' ORDER BY ID ASC;";
    rc = sqlite3_exec(db, (const char*)query.c_str(), parse_inputs, &tx, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }
}



int DB_operations::parse_transaction(void* tx, int argc, char** argv, char** azColName) {

    Transaction* tx_ptr = (Transaction*)tx;
    tx_ptr->hash = argv[0] ? argv[0] : "";
    tx_ptr->version = argv[1] ? argv[1] : "-.-.-.-";
    tx_ptr->time = argv[2] ? argv[2] : "";
    tx_ptr->origin = argv[3] ? argv[3] : "";
    tx_ptr->fee = argv[4] ? std::stof(argv[4]) : 0.f;
    tx_ptr->signature = argv[5] ? argv[5] : "";
    return 0;
}


void DB_operations::get_block(Block& blk, int block_id) {
    std::string query = "SELECT * FROM BLOCKS WHERE BLOCK_ID=" + std::to_string(block_id) + ";";
    //Block blk;
    int num_tx = 0;

    int rc = sqlite3_exec(db, (const char*)query.c_str(), parse_block, &blk, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }

    query = "SELECT HASH, VERSION, TIME, OWNER, FEE, SIGNATURE FROM TRANSACTIONS WHERE BLOCK =" + std::to_string(block_id) + " ORDER BY ID ASC;";
    rc = sqlite3_exec(db, (const char*)query.c_str(), parse_transactions, &(blk), &zErrMsg);
    
    for (int i = 0; i < blk.transaction_list.size(); i++) {

        query = "SELECT ACCOUNT, VALUE FROM OUTPUTS WHERE TX_HASH='"+ blk.transaction_list[i].hash +"' ORDER BY ID ASC;";
        rc = sqlite3_exec(db, (const char*)query.c_str(), parse_outputs, &blk.transaction_list[i], &zErrMsg);
        if (rc != SQLITE_OK) {
            std::cerr << "SQL error: " << zErrMsg << std::endl;
            sqlite3_free(zErrMsg);
        }

        query = "SELECT INPUT FROM INPUTS WHERE TX_HASH='" + blk.transaction_list[i].hash + "' ORDER BY ID ASC;";
        rc = sqlite3_exec(db, (const char*)query.c_str(), parse_inputs, &blk.transaction_list[i], &zErrMsg);
        if (rc != SQLITE_OK) {
            std::cerr << "SQL error: " << zErrMsg << std::endl;
            sqlite3_free(zErrMsg);
        }
    }

   
}

int DB_operations::parse_block(void* blk, int argc, char** argv, char** azColName) {
 
    
    //BLOCK_ID, VERSION, TIME, FATHER_HASH, WORK_HASH, MINER, REWARD, MT_ROOT, NONCE
    Block* blk_ptr = (Block*)blk;
    blk_ptr->ID = argv[0] ? (unsigned int)std::stoul(argv[0]) : 0;
    blk_ptr->version = argv[1] ? argv[1] : "-.-.-.-";
    blk_ptr->time = argv[2] ? argv[2] : "";
    blk_ptr->father_hash = argv[3] ? argv[3] : "";
    blk_ptr->work_hash = argv[4] ? argv[4] : "";
    blk_ptr->miner = argv[5] ? argv[5] : "";
    blk_ptr->reward = argv[6] ? std::stof(argv[6]) : 0.f;
    blk_ptr->mt_root = argv[7] ? argv[7] : "";
    blk_ptr->nonce = argv[8] ? (unsigned int)std::stoul(argv[8]) : 0;
    
    return 0;
}

int DB_operations::parse_transactions(void* tx_list, int argc, char** argv, char** azColName) {

    //Transaction tx;
    Block* tx_ptr = (Block*)tx_list;
    //std::vector<Transaction>* tx_ptr = (std::vector<Transaction>*)tx_list;
    tx_ptr->transaction_list.push_back(Transaction());
    //Transaction* tx_ptr = (Transaction*)tx_list;

    tx_ptr->transaction_list.back().hash = argv[0] ? argv[0] : "";
    tx_ptr->transaction_list.back().version = argv[1] ? argv[1] : "";
    tx_ptr->transaction_list.back().time = argv[2] ? argv[2] : "";
    tx_ptr->transaction_list.back().origin = argv[3] ? argv[3] : "";
    tx_ptr->transaction_list.back().fee = argv[4] ? std::stof(argv[4]) : 0.f;
    tx_ptr->transaction_list.back().signature = argv[5] ? argv[5] : "";

    //blk_ptr->transaction_list.push_back(Transaction(tx));

    return 0;
}

int DB_operations::parse_outputs(void* tx, int argc, char** argv, char** azColName) {
    
    Transaction* tx_ptr = (Transaction*)tx;
    tx_ptr->outputs.push_back(Entity(argv[0],std::stof(argv[1])));
 
    return 0;
}

int DB_operations::parse_inputs(void* tx, int argc, char** argv, char** azColName) {

    Transaction* tx_ptr = (Transaction*)tx;
    tx_ptr->inputs.push_back(argv[0]);

    return 0;
}

float DB_operations::get_balance(std::string account) {
    float balance = 0;
    //std::string query = "SELECT VALUE FROM OUTPUTS WHERE ACCOUNT='" + account +"' AND TX_HASH NOT IN (SELECT INPUT FROM INPUTS WHERE TX_HASH IN (SELECT HASH FROM TRANSACTIONS WHERE OWNER='" + account + "'));";
    std::string query = "SELECT VALUE FROM OUTPUTS WHERE ACCOUNT='"+account+"' AND TX_HASH NOT IN (SELECT DISTINCT ins.INPUT FROM INPUTS ins, TRANSACTIONS tx WHERE tx.HASH = ins.TX_HASH AND tx.OWNER ='"+account+"'); ";
    int rc;

    rc = sqlite3_exec(db, (const char*)query.c_str(), callback_balance, &balance, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }

    query = "SELECT REWARD FROM BLOCKS WHERE MINER='" + account + "' AND WORK_HASH NOT IN (SELECT INPUT FROM INPUTS); ";
    rc = sqlite3_exec(db, (const char*)query.c_str(), callback_balance, &balance, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }

    return balance;
}


int DB_operations::callback_balance(void* balance, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        *(float*)balance += std::stof(argv[i]);
    }
    //*(float*)balance = std::stof(argv[0]);
    return 0;
}

void DB_operations::get_inputs(std::vector<Entity>& inputs_full, std::string address, std::string time) {
    std::string query = "SELECT TX_HASH, VALUE FROM OUTPUTS WHERE ACCOUNT='" + address + "' AND TX_HASH NOT IN (SELECT DISTINCT ins.INPUT FROM INPUTS ins, TRANSACTIONS tx WHERE tx.HASH = ins.TX_HASH AND tx.OWNER ='" + address + "' AND TIME < '" + time + "'); ";
    int rc;

    rc = sqlite3_exec(db, (const char*)query.c_str(), callback_inputs, &inputs_full, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }

    query = "SELECT WORK_HASH, REWARD FROM BLOCKS WHERE MINER='" + address + "' AND WORK_HASH NOT IN (SELECT INPUT FROM INPUTS WHERE TX_HASH IN (SELECT HASH FROM TRANSACTIONS WHERE TIME < '" + time + "')); ";
    rc = sqlite3_exec(db, (const char*)query.c_str(), callback_inputs, &inputs_full, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }
}

int DB_operations::callback_inputs(void* inputs_full, int argc, char** argv, char** azColName) {
    std::vector<Entity>* inputs_full_ptr = (std::vector<Entity>*)inputs_full;
    std::cout << "input: " << argv[0] << ", " << argv[1] << std::endl;
    for (int i = 0; i < inputs_full_ptr->size(); i++) {
        
        if (argv[0] == (*inputs_full_ptr)[i].account) {
            
            (*inputs_full_ptr)[i].value = std::stof(argv[1]);
            return 0;
        }
    }
    return 0;
}


unsigned int DB_operations::get_head() {
    unsigned int head;
    std::string query = "SELECT MAX(BLOCK_ID) FROM BLOCKS;";
    int rc;

    rc = sqlite3_exec(db, (const char*)query.c_str(), callback_head, &head, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }

    return head;
}


int DB_operations::callback_head(void* head, int argc, char** argv, char** azColName) {
    *(unsigned int*)head = argv[0] ? (unsigned int)std::stoul(argv[0]) : 0;
    return 0;
}


std::string DB_operations::select(Element table, std::string data) {

    std::string query = "SELECT * FROM ";
    int rc;

    if (table == Element::OUTPUTS) {
        query += "OUTPUTS (TX_ID, ACCOUNT, VALUE) VALUES (" + data + "); ";
    }
    else if (table == Element::TRANSACTIONS) {
        query += "TRANSACTIONS ;";
    }
    else if (table == Element::BLOCKS) {
        query += "BLOCKS (BLOCK_ID, VERSION, WORK_HASH, FATHER_HASH, NONCE, TIME, MINER) VALUES (" + data + "); ";
    }


    rc = sqlite3_exec(db, (const char*)query.c_str(), callback, 0, &zErrMsg);


    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }
    else {
        std::cout << "Insertion success" << std::endl;
    }

 
    return "hola";
}

DB_operations::~DB_operations() {
    sqlite3_close(db);
}

