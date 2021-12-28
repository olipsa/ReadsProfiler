//
// Created by ondina on 20.12.2021.
//

#include "Download.h"
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

Download::Download(vector<string> arguments,Server *server) : Command(arguments,server) {}

string Download::Execute() {
    if(arguments.size()!=1)
        return "To download a book you should use the following syntax: 'download [book_isbn]'";
    string isbn = arguments[0];
    if(!BookFound(isbn))
        return "No book available with the isbn "+isbn;
    else{
        string sql = "SELECT image FROM books WHERE isbn="+isbn;
        if(db.GetQueryResults(sql,query_results)) {
            if(!query_results.empty()){
                return query_results[0][0];
            }
        }
    }
}
