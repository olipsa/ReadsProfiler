//
// Created by ondina on 26.08.2021.
//
#pragma once
#include "Command.h"
#include "../Server.h"
#include <string>
#include <vector>

class Register:public Command {
public:
    Register(std::vector <std::string>, Server*);
    std::string Execute();
};

