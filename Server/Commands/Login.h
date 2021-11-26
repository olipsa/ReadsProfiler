//
// Created by ondina on 25.11.2021.
//
#pragma once
#include "Register.h"
#include "../Server.h"
#include <string>
#include <vector>


class Login:public Command {
    std::vector<std::string>arguments;
    Server *server;
public:
    Login(std::vector <std::string>, Server*);
    std::string Execute();
};


