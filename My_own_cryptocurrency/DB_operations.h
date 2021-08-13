#pragma once


class DB_operations {

   
    /// <summary>
    /// Fuction called after quering the database.
    /// </summary>
    static int callback(void* NotUsed, int argc, char** argv, char** azColName);

    /// <summary>
    /// Creating tables for the database
    /// </summary>
    void create_tables();


public:

    /// <summary>
    /// Opening database
    /// </summary>
    DB_operations();

    /// <summary>
    /// Inserting block into the database.
    /// </summary>
    /// <param name="table">Table where the data is gonna be inserted.</param>
    /// <param name="data">List of parameter to be inserted in each row.</param>
    void insert(std::string table, std::string data);

    /// <summary>
    /// Destroying instance and closing database.
    /// </summary>
    /// <param name="table"></param>
    /// <param name="data"></param>
    ~DB_operations();
};