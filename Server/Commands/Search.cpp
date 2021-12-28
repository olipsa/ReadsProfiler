//
// Created by ondina on 26.11.2021.
//

#include "Search.h"
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

Search::Search(vector<string> arguments,Server *server) : Command(arguments,server) {}

vector<pair<int,string>>Search::get_search_result(){
    return search_result;
}

string Search::Execute(){
    if(arguments.size()<2)
        return "After search you should type the criterion to search for and it's value.";
    for(int i=0;i<arguments.size();i++)
        transform(arguments[i].begin(),arguments[i].end(),arguments[i].begin(),[](unsigned char c){return tolower(c);});
    string criterion = arguments[0];
    string sql;
    if(criterion=="title" ||criterion=="author"){
        sql = "SELECT * FROM books WHERE "+criterion+" LIKE '%"+arguments[1]+"%'";
        for(int i=2;i<arguments.size();i++)
            sql += " AND "+criterion+" LIKE '%"+arguments[i]+"%'";
    }
    else if(criterion=="year" || criterion=="rating" || criterion=="isbn")
        sql = "SELECT * FROM books WHERE "+criterion+"="+arguments[1];
    else
        return "You can search books by title, author, year, rating or ISBN number.";
    Database db =server->getDatabase();
    vector<vector<string>> query_results;
    if(db.GetQueryResults(sql,query_results)){
        if(query_results.empty())
            return "No results found.";
        else{
            string books_found="";
            for(int i=0;i<query_results.size();i++){
                books_found += to_string(i+1)+". \"";
                string title = query_results[i][0];
                string author = query_results[i][1];
                string isbn = query_results[i][3];
                search_result.push_back({i+1,isbn});
                bool has_collection = false;
                for(int j=0;j<title.size();j++) {
                    if (title[j + 1] == '(') {
                        books_found += "\"";
                        has_collection = true;
                    }
                    books_found +=title[j];
                }
                if(has_collection==false)
                    books_found += "\"";
                books_found += " by "+author+"\n";
            }
            return books_found;
        }

    }
    else
        return "Internal error occurred.";
}