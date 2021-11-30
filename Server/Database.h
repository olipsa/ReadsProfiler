//
// Created by ondina on 02.08.2021.
//
#include<vector>
#include <sqlite3.h>

class Database {
    sqlite3 *database;
    char * db_error;
    int status;
public:
    Database();
    ~Database();
    bool OpenDatabase(const char *);
    bool ExecuteQuery(const std::string &);
    static int callback(void *data, int argc, char **argv, char **azColName);
    bool GetQueryResults(const std::string &,std::vector<std::vector<std::string>>&);
    void CloseDatabase();

};


