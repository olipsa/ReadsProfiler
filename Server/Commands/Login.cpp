//
// Created by ondina on 25.11.2021.
//

#include "Login.h"
#include <iostream>
#include <vector>
using namespace std;
Login::Login(vector<string> arguments,Server *server) : Command(arguments,server) {}
string Login::Execute() {
    if(arguments.size()!=2)
        return "After login you should type your username and password separated by space.";
    string username=arguments[0];
    string password=arguments[1];
    string sql="SELECT * FROM users WHERE username =\""+username+"\"";
    vector<vector<string>> query_results;
    if(db.GetQueryResults(sql,query_results)) {
        if(!query_results.empty()){
            if(query_results[0][1]==password){
                return "Logged in with success.\nWelcome, "+username+"!";
            }

            else
                return "Incorrect password. Please try again.";
        }
    }
    return "Username doesn't exist";
}
