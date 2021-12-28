//
// Created by ondina on 20.12.2021.
//
#pragma once
#include "Register.h"
#include "../Server.h"
#include <string>
#include <vector>

class Upload:public Command {
public:
    Upload(std::vector <std::string>, Server*);
    std::string Execute();
};

