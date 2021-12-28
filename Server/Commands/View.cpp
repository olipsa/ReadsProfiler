//
// Created by ondina on 28.12.2021.
//

#include "View.h"
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

View::View(vector<string> arguments,Server *server, vector<pair<int,string>> last_search_result) : Command(arguments,server) {}
