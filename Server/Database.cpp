//
// Created by ondina on 02.08.2021.
//
#include <iostream>
#include "Database.h"
using namespace std;
Database::Database() {}
bool Database::OpenDatabase(const char *filename) {
    if(sqlite3_open(filename,&database)) {
        cout<<"Unable to open database\n";
        return false;
    }
    cout<<"Database "<<filename<<" opened.\n";
    return true;
}
static int callback(void *data, int argc, char **argv, char **azColName){
    cout<<"\nhere\n";
    Data * d=(Data *)data;
    d->rows=argc;
    for(int i=0;i<argc;i++){
        if(data!=nullptr)
            d->vector.push_back((argv[i]?argv[i]:"NULL"));
    }
    cout<<d->rows<<endl;
    for(int i=0;i<d->vector.size();i++)
        cout<<d->vector[i];
    cout<<endl;
    return 0;
}
bool Database::ExecuteQuery(const std::string & query) {
    if(sqlite3_exec(database, query.c_str(), callback,0, &db_error)!=SQLITE_OK){
        cout<<"Couldn't execute following query: "<<query<<endl;
        sqlite3_free(db_error);
        return false;
    }
    cout<<"Query executed\n";
    return true;


}
void Database::CloseDatabase() {
    sqlite3_close(database);
}
