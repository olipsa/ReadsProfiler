//
// Created by ondina on 28.12.2021.
//
#pragma once
#include "Register.h"
#include "../Server.h"
#include <string>
#include <vector>


class View:public Command {
public:
    View(std::vector <std::string>, Server*);
    std::string Execute();
};


