#ifndef __HTTP_CPP_CLASS__
#define __HTTP_CPP_CLASS__
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <iterator>
#include <sstream> 
#include <fstream> 
#include <map>
#include <sstream>
#include "HTTP.hpp"
/*
Removing a leading whitespace
*/
std::string & ltrim(std::string & str)
{
  if(*str.begin()==' '){
  str.erase( str.begin());
  }
  return str;   
}
std::string HTTP::httpGet(std::string name){
    return this->getparams[name];
}
std::string HTTP::httpPost(std::string name){
    return this->postparams[name];
}
std::string HTTP::getCookie(std::string name){
    return this->cookie[name];
}
std::string HTTP::setCookie(std::string name, std::string value){
    this->outcookie[name]=value;
    return value;
}
void HTTP::init(){
    //redirecting cout stream to string stream
    std::streambuf *x = std::cout.rdbuf( this->OUT.rdbuf( ) );
}
void HTTP::send(){
    //set headers
    fprintf( stdout, "Content-Type: text/html; charset=utf-8\n");
    //set cookie
    fprintf( stdout, "Set-Cookie: ");
    
    for (const auto& [key, value] : this->outcookie)
        fprintf(stdout,"%s=%s; ",key.c_str(),value.c_str());
        
    fprintf( stdout, "\n\n");
    //output recorded cout
    fprintf( stdout, "%s", OUT.str().c_str());
}
HTTP::HTTP(std::string query, std::string postdata){
    std::vector<std::string> out;
    std::stringstream getstrstm(query);
    std::stringstream poststrstm(postdata);
    auto key = std::string{};
    auto val = std::string{};

    while (std::getline( getstrstm, key, '=' )) {
        std::getline( getstrstm, val, '&' );
        this->getparams[key]=val;
    }
    while (std::getline( poststrstm, key, '=' )) {
        std::getline( poststrstm, val, '&' );
        this->postparams[key]=val;
    }
return;
}
HTTP::HTTP(){
    std::stringstream getstrstm(getenv("QUERY_STRING"));
    std::string postdata; 
    std::getline(std::cin,postdata,char(0));
    std::stringstream poststrstm(postdata);
    auto key = std::string{};
    auto val = std::string{};

    //parse GET params
    while (std::getline( getstrstm, key, '=' )) {
        std::getline( getstrstm, val, '&' );
        this->getparams[key]=val;
    }
    //parse POST params
    while (std::getline( poststrstm, key, '=' )) {
        std::getline( poststrstm, val, '&' );
        this->postparams[key]=val;
    }
    //parse cookie
    if(getenv("HTTP_COOKIE") != nullptr){
        std::stringstream cookiestrstm(getenv("HTTP_COOKIE"));
        while (std::getline( cookiestrstm, key, '=' )) {
            std::getline( cookiestrstm, val, ';' );
            key = ltrim(key);
            this->cookie[key]=val;
        }
    }
    return;
}
int HTTP::httpSendFile(std::string name){
    std::ifstream ifile;
    ifile.open(name, std::ios::in); 
    if(!ifile){
        return -1;
    }
    std::string str;
    while (std::getline(ifile, str,'\n')) {
        std::cout<<str<<std::endl;
    }
    return 0;
}

HTTP::~HTTP(){
    return;
}

#endif