//
// Created by ondina on 26.08.2021.
//
#include<string>
#include<vector>
#include "../Server.h"


class Command {
protected:
    vector<std::string>arguments;
    Server *server;
    vector<vector<string>> query_results;
    Database db;
public:
    Command();
    Command(std::vector <std::string>, Server*);
    virtual std::string Execute()=0;
    bool BookFound(string);


};

