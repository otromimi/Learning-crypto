#include "sqlite3.h"
#include <stdio.h>
#include <stdlib.h>




class DB_operations {

    sqlite3* db;
    char* zErrMsg = 0;
    int rc;

    const char* transactions_table = "CREATE TABLE TRANSACTIONS("  \
        "TX_ID          CHAR(32) PRIMARY KEY     NOT NULL," \
        "VERSION        TEXT                     NOT NULL," \
        "INPUTS         TEXT                     NOT NULL," \
        "DESTINATION    CHAR(32)                 NOT NULL," \
        "VALUE          INT                      NOT NULL," \
        "OWNER          CHAR(32)                 NOT NULL," \
        "CASHBACK       INT                      NOT NULL," \
        "FEE            INT                      NOT NULL," \
        "SIGNATURE      CHAR(32)                 NOT NULL);";

   
    const char* blocks_table = "CREATE TABLE BLOCKS("  \
        "BLOCK_ID       INT PRIMARY KEY          NOT NULL," \
        "VERSION        TEXT                     NOT NULL," \
        "WORK_HASH      CHAR(32)                 NOT NULL," \
        "FATHER_HASH    CHAR(32)                 NOT NULL," \
        "NONCE          INT                      NOT NULL," \
        "TIME           INT                      NOT NULL);";

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
        rc = sqlite3_exec(db, (const char*)(std::string(transactions_table) + blocks_table).c_str(), callback, 0, &zErrMsg);
        std::cout <<transactions_table << std::endl;

        if (rc != SQLITE_OK) {
            std::cerr << "SQL error: " << zErrMsg << std::endl;
            sqlite3_free(zErrMsg);
        }
        else {
            std::cout << "Tables created successfully" << std::endl;
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

        /*Create SQL statement */
        const char* sql = transactions_table;

        /* Execute SQL statement */
        rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        }
        else {
            fprintf(stdout, "Table created successfully\n");
        }

    }


    ~DB_operations() {
        sqlite3_close(db);
    }

};