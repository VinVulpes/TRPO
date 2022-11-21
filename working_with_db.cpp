#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>
#include "HTTP.hpp"
#include "db.hpp"
#include "db.cpp"
using namespace std;
int main()
{
    HTTP http;
    http.init();
    FILEDB<std::string,std::string> db;
    std::string getparam = string(http.httpGet("get"));
    std::string getallparam = string(http.httpGet("getall"));
    if(getparam != ""){
        std::string res = db.read_one(getparam);
        if(res != ""){
            cout <<"{ \""<<getparam<<"\": \""<< res <<"\"}";
        }
    }
    if(getallparam != ""){
        
        std::map<string,string> *res = db.read_all();
        cout<<"{";
        int i = 0;
        for(auto [key,val]: *res){
            if(i>0){
                cout<<", ";
            }
            cout <<"\""<<key<<"\": \""<< val <<"\"";
            i++;
        }
        cout<<"}";
    }

    http.setCookie("get",getparam);
    http.send();
}