//
// Created by ondina on 26.08.2021.
//
#include<string>
#include<vector>
#include "../Server.h"


class Command {
protected:
    vector<std::string>arguments;
    string username;
    Server *server;
    Database db;
public:
    Command();
    Command(std::vector <std::string>, Server*);
    virtual std::string Execute()=0;
    bool BookFound(string);
    void AddUserActivity(string activity_type, string isbn);


};

