//
// Created by ondina on 26.08.2021.
//

#include "Command.h"
using namespace std;
Command::Command() {}
Command::Command(vector<string> arguments,Server *server) {
    this->arguments=arguments;
    this->server=server;
    db=server->getDatabase();
}
bool Command::BookFound(string isbn){
    string sql = "SELECT * FROM books WHERE ISBN="+isbn;
    Database db = server->getDatabase();
    vector<vector<string>> query_results;
    if(db.GetQueryResults(sql,query_results))
        if(!query_results.empty())
            return true;
    return false;
}

