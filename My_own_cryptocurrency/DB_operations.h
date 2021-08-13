#pragma once


class DB_operations {



    static int callback(void* NotUsed, int argc, char** argv, char** azColName);

    void create_tables();


public:
    DB_operations();


    void insert(std::string table, std::string data);


    ~DB_operations();
};