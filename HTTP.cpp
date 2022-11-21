#ifndef __HTTP_CPP_CLASS__
#define __HTTP_CPP_CLASS__
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <iterator>
#include <sstream> 
#include <map>
#include <sstream>
#include "HTTP.hpp"
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
    std::streambuf *x = std::cout.rdbuf( this->OUT.rdbuf( ) );
}
void HTTP::send(){
    fprintf( stdout, "Content-Type: text/html; charset=utf-8\n");
    fprintf( stdout, "Set-Cookie: ");
    
    for (const auto& [key, value] : this->outcookie)
        fprintf(stdout,"%s=%s; ",key.c_str(),value.c_str());
        
    fprintf( stdout, "\n\n");
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
    char postdata[2000]; 
    std::cin.getline(postdata,2000);
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
HTTP::~HTTP(){
    return;
}

#endif