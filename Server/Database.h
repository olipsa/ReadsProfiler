//
// Created by ondina on 02.08.2021.
//
#include<vector>
#include <sqlite3.h>
struct Data {
    int rows = 0;
    std::vector<std::string> vector;
};

class Database {
    sqlite3 *database;
    char * db_error;
    int status;
public:
    Database();
    bool OpenDatabase(const char *);
    bool ExecuteQuery(const std::string &);
    void CloseDatabase();

};

