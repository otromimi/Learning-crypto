#include "sqlite3.h"
#include <stdio.h>
#include <stdlib.h>




class DB_operations {

	sqlite3* db;
	char* zErrMsg = 0;
	int rc;

    const char* sql;

    static int callback(void* NotUsed, int argc, char** argv, char** azColName) {
        int i;
        for (i = 0; i < argc; i++) {
            printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        }
        printf("\n");
        return 0;
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

        /* Create SQL statement */
        sql = "CREATE TABLE COMPANY("  \
            "ID INT PRIMARY KEY     NOT NULL," \
            "NAME           TEXT    NOT NULL," \
            "AGE            INT     NOT NULL," \
            "ADDRESS        CHAR(50)," \
            "SALARY         REAL );";

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