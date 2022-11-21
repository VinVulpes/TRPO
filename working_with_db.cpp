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
        http.setCookie("get",getparam);
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
    std::string setparam = string(http.httpGet("set"));
    if(setparam != ""){
        int res = db.write(setparam,string(http.httpGet("value")));
        if(res == 0){
            cout<<"201 OK";
        }
        else{
            cout<<"500";

        }
    }

    
    http.send();
}