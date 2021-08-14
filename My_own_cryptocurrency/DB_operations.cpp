// Standard dependencies
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

// My own dependencies
#include "DB_operations.h"




/// Queries

const char* select_tx = "SELECT * FROM TRANSACTIONS;";

const char* transactions_table = "CREATE TABLE TRANSACTIONS("  \
"SIGNATURE      TEXT PRIMARY KEY         NOT NULL," \
"BLOCK          INT                      NOT NULL," \
"TIME           INT                      NOT NULL," \
"ORIGIN         TEXT                     NOT NULL," \
"FEE            INT                      NOT NULL," \
"FOREIGN KEY (BLOCK) REFERENCES BLOCKS(BLOCK_ID)" \
"ON DELETE CASCADE ON UPDATE CASCADE); ";

const char* outputs_table = "CREATE TABLE OUTPUTS("  \
"TX_ID          TEXT PRIMARY KEY         NOT NULL," \
"ACCOUNT        INT                      NOT NULL," \
"VALUE          INT                      NOT NULL," \
"FOREIGN KEY (TX_ID) REFERENCES TRANSACTIONS(SIGNATURE)" \
"ON DELETE CASCADE ON UPDATE CASCADE); ";



const char* blocks_table = "CREATE TABLE BLOCKS("  \
"BLOCK_ID       INT PRIMARY KEY          NOT NULL," \
"VERSION        INT                      NOT NULL," \
"WORK_HASH      TEXT                     NOT NULL," \
"FATHER_HASH    TEXT                     NOT NULL," \
"NONCE          INT                      NOT NULL," \
"TIME           INT                      NOT NULL," \
"MINER          TEXT                     NOT NULL);";




int DB_operations::callback(void* NotUsed, int argc, char** argv, char** azColName) {
    int i;
    for (i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}


void DB_operations::create_tables() {

    /* Execute SQL statement */

    int rc;

    rc = sqlite3_exec(db, (const char*)blocks_table, callback, (void*)data, &zErrMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL log: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }
    else {
        std::cout << "SQL log: " << "tables BLOCKS created successfully" << std::endl;
    }

    rc = sqlite3_exec(db, (const char*)transactions_table, callback, 0, &zErrMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL log: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }
    else {
        std::cout << "SQL log: " << "table TRANSACTIONS created successfully" << std::endl;
    }

    rc = sqlite3_exec(db, (const char*)outputs_table, callback, 0, &zErrMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL log: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }
    else {
        std::cout << "SQL log: " << "table OUTPUTS created successfully" << std::endl;
    }


}



DB_operations::DB_operations() {

    int rc;

    rc = sqlite3_open("my_blockchain.db", &db);

    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    }
    else {
        fprintf(stdout, "Opened database successfully\n");
    }


    /* create tables */
    create_tables();


}


void DB_operations::insert(Element table, std::string data) {

    std::string query = "INSERT INTO ";
    int rc;

    if (table == Element::ENTITY) {
        query += "OUTPUTS (TX_ID, ACCOUNT, VALUE) VALUES (" + data + "); ";
    }
    else if (table == Element::TRANSACTION) {
        query += "TRANSACTIONS (SIGNATURE, BLOCK, TIME, ORIGIN, FEE) VALUES (" + data + "); ";
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
}

std::string DB_operations::select(Element table, std::string data) {

    std::string query = "SELECT * FROM ";
    int rc;

    if (table == Element::ENTITY) {
        query += "OUTPUTS (TX_ID, ACCOUNT, VALUE) VALUES (" + data + "); ";
    }
    else if (table == Element::TRANSACTION) {
        query += "TRANSACTIONS (SIGNATURE, BLOCK, TIME, ORIGIN, FEE) VALUES (" + data + "); ";
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

