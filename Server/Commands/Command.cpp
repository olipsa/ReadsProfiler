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
    vector<vector<string>> query_results;
    string sql = "SELECT * FROM books WHERE ISBN="+isbn;
    if(db.GetQueryResults(sql,query_results))
        if(!query_results.empty())
            return true;
    return false;
}
void Command::AddUserActivity(string activity_type, string isbn) {
    string sql = "SELECT " + activity_type + " FROM user_activity WHERE username=\"" + username + "\"";
    vector<vector<string>> query_results;
    if (db.GetQueryResults(sql, query_results))
        if (!query_results.empty()){
            string updated_activity= (query_results[0][0]=="NULL")?isbn:(query_results[0][0]+"_"+isbn);
            if(updated_activity.size()>4000) //300 books
                updated_activity.erase(0,14); //removes the oldest book searched
            string sql2 = "UPDATE user_activity SET "+activity_type+"=\""+updated_activity+"\" WHERE username =\"" + username + "\"";
            if(!db.ExecuteQuery(sql2))
                cout<<"Error occurred when adding activity for "+username<<endl;
            else
                cout<<"User activity updated for user "+username<<endl;
        }
        else{
            cout<<"Username "+username+" doesn't exist.\n";
        }
}

