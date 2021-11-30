//
// Created by ondina on 26.11.2021.
//

#pragma once
#include "Register.h"
#include "../Server.h"
#include <string>
#include <vector>


class Search:public Command {
    std::vector<std::string>arguments;
    Server *server;
public:
    Search(std::vector <std::string>, Server*);
    std::string Execute();
};


