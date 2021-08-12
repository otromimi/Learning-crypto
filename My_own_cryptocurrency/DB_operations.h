#pragma once

// Standard dependencies
#include <stdio.h>
#include <stdlib.h>

// My own dependencies


// Local dependencies
#include "sqlite3.h"







class DB_operations {

    sqlite3* db;
    char* zErrMsg = 0;
    int rc;

    /*std::string version;
    std::string time;
    std::vector<std::string> inputs;
    std::vector<Entity> outputs;
    std::string origin; // Transation owner PK
    unsigned int fee;
    std::string signature;*/
    

    const char* transactions_table = "CREATE TABLE TRANSACTIONS("  \
        "SIGNATURE      TEXT PRIMARY KEY         NOT NULL," \
        "BLOCK          INT                      NOT NULL," \
        "TIME           INT                      NOT NULL," \
        //"INPUTS         TEXT                     NOT NULL," \//
        //"OUTPUTS        TEXT                     NOT NULL," \//
        "ORIGIN         TEXT                      NOT NULL," \
        "FEE            INT                      NOT NULL);"; 

    const char* outputs_table = "CREATE TABLE OUTPUTS("  \
        "TX_ID          TEXT PRIMARY KEY         NOT NULL," \
        "ACCOUNT        INT                      NOT NULL," \
        "VALUE          INT                      NOT NULL);";


    /*
    // Proof of work fields
    std::string father_hash;
    std::string time;
    std::string miner;
    unsigned int reward;
    std::vector<Transaction> transaction_list;
    unsigned int ID;
    unsigned int nonce;
    // Non hased fields
    std::string work_hash;*/
   
    const char* blocks_table = "CREATE TABLE BLOCKS("  \
        "BLOCK_ID       INT PRIMARY KEY          NOT NULL," \
        "VERSION        INT                      NOT NULL," \
        "WORK_HASH      TEXT                     NOT NULL," \
        "FATHER_HASH    TEXT                     NOT NULL," \
        "NONCE          INT                      NOT NULL," \
        "TIME           INT                      NOT NULL," \
        "MINER          TEXT                     NOT NULL);";

    static int callback(void* NotUsed, int argc, char** argv, char** azColName) {
        int i;
        for (i = 0; i < argc; i++) {
            printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        }
        printf("\n");
        return 0;
    }

    void create_tables() {

        /* Execute SQL statement */
        rc = sqlite3_exec(db, (const char*)transactions_table, callback, 0, &zErrMsg);

        if (rc != SQLITE_OK) {
            std::cerr << "SQL error: " << zErrMsg << std::endl;
            sqlite3_free(zErrMsg);
        }
        else {
            std::cout << "Table TRANSACTIONS created successfully" << std::endl;
        }

        rc = sqlite3_exec(db, (const char*)blocks_table, callback, 0, &zErrMsg);

        if (rc != SQLITE_OK) {
            std::cerr << "SQL error: " << zErrMsg << std::endl;
            sqlite3_free(zErrMsg);
        }
        else {
            std::cout << "Tables BLOCKS created successfully" << std::endl;
        }


        rc = sqlite3_exec(db, (const char*)outputs_table, callback, 0, &zErrMsg);
       

        if (rc != SQLITE_OK) {
            std::cerr << "SQL error: " << zErrMsg << std::endl;
            sqlite3_free(zErrMsg);
        }
        else {
            std::cout << "Table OUTPUTS created successfully" << std::endl;
        }

        
    }


public:
    DB_operations() {
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


    void insert(std::string table, std::string data) {

        std::string query = "INSERT INTO ";
        query += table + " (SIGNATURE, BLOCK, TIME, ORIGIN, FEE) VALUES (" + data + "); ";


        rc = sqlite3_exec(db, (const char*)query.c_str(), callback, 0, &zErrMsg);


        if (rc != SQLITE_OK) {
            std::cerr << "SQL error: " << zErrMsg << std::endl;
            sqlite3_free(zErrMsg);
        }
        else {
            std::cout << "Insertion success" << std::endl;
        }
    }


    ~DB_operations() {
        sqlite3_close(db);
    }

};