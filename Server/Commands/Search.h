//
// Created by ondina on 26.11.2021.
//

#pragma once
#include "Register.h"
#include "../Server.h"
#include <string>
#include <vector>


class Search:public Command {
    vector<pair<int,string>> search_result;
public:
    Search(std::vector <std::string>, Server*);
    vector<pair<int,string>> get_search_result();
    std::string Execute();
};


