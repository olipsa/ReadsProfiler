//
// Created by ondina on 02.08.2021.
//
#include <iostream>
#include "Database.h"
using namespace std;
Database::Database() {}
Database::~Database() {}
bool Database::OpenDatabase(const char *filename) {
    if(sqlite3_open(filename,&database)) {
        cout<<"Unable to open database\n";
        return false;
    }
    cout<<"Database "<<filename<<" opened.\n";
    return true;
}
bool Database::ExecuteQuery(const std::string & query) {
    if(sqlite3_exec(database, query.c_str(), callback,0, &db_error)!=SQLITE_OK){
        cout<<"Couldn't execute following query: "<<query<<endl;
        sqlite3_free(db_error);
        return false;
    }
    cout<<"Query "<<query<<" executed\n";
    return true;
}
void Database::CloseDatabase() {
    sqlite3_close(database);
}
bool Database::GetQueryResults(const std::string & query, vector<vector<string>>&query_result){
    if(sqlite3_exec(database,query.c_str(),callback,&query_result,&db_error)!=SQLITE_OK){
        cout<<"Couldn't execute following query: "<<query<<endl;
        sqlite3_free(db_error);
        return false;
    }
    cout<<"Query "<<query<<" executed successfully.\n";
    return true;
}
int Database::callback(void *data, int argc, char **argv, char **azColName){
    vector<vector<string>>*query_result=(vector<vector<string>>*)data;
    vector<string>line;
    for(int i=0;i<argc;i++)
        line.push_back((argv[i]?argv[i]:"NULL"));
    query_result->push_back(line);
    return 0;
}
