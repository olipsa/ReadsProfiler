//
// Created by ondina on 28.12.2021.
//

#include "View.h"
#include <iostream>
#include <vector>
using namespace std;

View::View(vector<string> arguments,Server *server, vector<string> last_search_result,string username) : Command(arguments,server) {
    search_result = last_search_result;
    this->username = username;
}

string View::Execute() {
    if(arguments.size()!=1)
        return "After view command you have to enter the number of the book to be viewed.";
    int book_no;
    try{
        book_no = stoi(arguments[0]);
    }catch(std::invalid_argument){
        return "Not a number.";
    }
    if(book_no<1 || book_no>search_result.size())
        return "Invalid book number. Please enter a value between 1 and "+ to_string(search_result.size())+".";
    string isbn = search_result[book_no-1];
    string sql = "SELECT * FROM books WHERE isbn="+isbn;
    vector<vector<string>> query_results;
    if(db.GetQueryResults(sql,query_results)){
        if(query_results.empty())
            return "Book was deleted.";
        else{
            vector<string> book_info = query_results[0];
            string title = book_info[0];
            string author = book_info[1];
            string year = book_info[2];
            string isbn = book_info[3];
            string rating = book_info[4];
            string url = book_info[5];
            string image = book_info[6];
            AddUserActivity("views",isbn);

            return title+" by "+author+
                    "\nPublished in "+year+
                    "\nISBN: "+isbn+
                    "\nRating: "+rating+
                    "\nBuy original book: "+url+
                    "\nBook cover: "+image+"\n";
        }
    }
    return "Internal error occurred.";
}
