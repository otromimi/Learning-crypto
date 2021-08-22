// Standard dependencies
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

// My own dependencies
#include "DB_operations.h"
#include "Structures.h"

using namespace My_own_crypto;

/// Queries

const char* select_tx = "SELECT * FROM TRANSACTIONS;";

const char* outputs_table = "CREATE TABLE OUTPUTS("  \
"ID             INTEGER PRIMARY KEY AUTOINCREMENT," \
"TX_HASH        TEXT                     NOT NULL," \
"ACCOUNT        TEXT                     NOT NULL," \
"VALUE          REAL                     NOT NULL," \
"FOREIGN KEY (TX_HASH) REFERENCES TRANSACTIONS(HASH)" \
"ON DELETE CASCADE ON UPDATE CASCADE); ";

const char* transactions_table = "CREATE TABLE TRANSACTIONS("  \
"HASH           TEXT PRIMARY KEY         NOT NULL," \
"VERSION        TEXT                     NOT NULL," \
"BLOCK          INT                      NOT NULL," \
"TIME           TEXT                     NOT NULL," \
"OWNER          TEXT                     NOT NULL," \
"FEE            REAL                     NOT NULL," \
"SIGNATURE      TEXT                     NOT NULL," \
"FOREIGN KEY (BLOCK) REFERENCES BLOCKS(BLOCK_ID)" \
"ON DELETE CASCADE ON UPDATE CASCADE); ";

const char* blocks_table = "CREATE TABLE BLOCKS("  \
"BLOCK_ID       INT PRIMARY KEY          NOT NULL," \
"VERSION        TEXT                     NOT NULL," \
"TIME           TEXT                     NOT NULL," \
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
}



DB_operations::DB_operations() {

    int rc = sqlite3_open("my_blockchain.db", &db);

    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
    }

    create_tables();
}


int DB_operations::insert(Element table, std::string data) const {

    std::string query = "INSERT INTO ";
 
    if (table == Element::ENTITY) {
        query += "OUTPUTS (TX_HASH, ACCOUNT, VALUE) VALUES (" + data + "); ";
    }
    else if (table == Element::TRANSACTION) {
        query += "TRANSACTIONS (HASH, VERSION, BLOCK, TIME, OWNER, FEE,  SIGNATURE) VALUES (" + data + "); ";
    }
    else if (table == Element::BLOCK) {
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
    int rc;
    std::string tx_hash;

    blk_sink << block.ID << ", \"" << block.version << "\", \"" << block.time << "\", \"" << block.father_hash << "\", \"" << block.work_hash << "\", \""
        << block.miner << "\", " << block.reward << ", \"" << block.mt_root << "\", " << block.nonce;

    rc = insert(Element::BLOCK, blk_sink.str());

    if (rc)
        return;

    for (Transaction tx : block.transaction_list) {
        tx_hash = tx.compute_hash();
        tx_sink << "\"" << tx_hash << "\", \"" << tx.version << "\", " << block.ID << ", \"" << tx.time << "\", \"" << tx.origin << "\", "
            << tx.fee << ", \"" << tx.signature << "\"";

        insert(Element::TRANSACTION, tx_sink.str());

        for (Entity out : tx.outputs) {
            output_sink << "\"" << tx_hash << "\", \"" << out.account << "\", " << out.value;
            
            insert(Element::ENTITY, output_sink.str());
            output_sink.str("");
        }
        tx_sink.str("");
    }
}





std::string DB_operations::select(Element table, std::string data) {

    std::string query = "SELECT * FROM ";
    int rc;

    if (table == Element::ENTITY) {
        query += "OUTPUTS (TX_ID, ACCOUNT, VALUE) VALUES (" + data + "); ";
    }
    else if (table == Element::TRANSACTION) {
        query += "TRANSACTIONS ;";
    }
    else if (table == Element::BLOCK) {
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

