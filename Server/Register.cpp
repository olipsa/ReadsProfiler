//
// Created by ondina on 26.08.2021.
//

#include "Register.h"
#include <iostream>
#include <vector>
using namespace std;

Register::Register(vector<string> arguments,Server *server) {
    this->arguments=arguments;
    this->server=server;
    cout<<"inside Register"<<endl;
}
string Register::Execute() {
    if(arguments.size()!=3)
        return "After register you should type your username and password separated by space.";
    string username=arguments[1];
    string password=arguments[2];
    string sql="SELECT * FROM users WHERE username =\" "+username+"\"";
    vector<string> query_results;
    Database db=server->getDatabase();
    if(db.GetQueryResults(sql,query_results)) {
        if(!query_results.empty())
            return "Username already exists.";
        sql="INSERT INTO users VALUES(\""+username+"\",\""+password+"\")";
        if(db.ExecuteQuery(sql))
            return "Account created.";

    }
    return "Account not created.";
}

