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
#include "HTTP.hpp"
/*
Removing a leading whitespace
*/
std::string & ltrim(std::string * str)
{
  if (*((*str).begin()) == ' '){
  (*str).erase((*str).begin());
  }
  if ((*((*str).end()-1) == '\n') or (*((*str).end()-1) == ' ')){
  (*str).erase((*str).end()-1);
    // (*str).pop_back();
  }
  return *str;
}
std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return str;
}
std::string HTTP::escaping(std::string str)
{
    // for()
    for (auto [key, val] : this->escape){
        str = ReplaceAll(str,key,val);
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
    this->outcookie[name] = value;
    return value;
}
void HTTP::init(){
    // redirecting cout stream to string stream
    std::streambuf *x = std::cout.rdbuf(this->OUT.rdbuf());
}
void HTTP::send(){
    // set headers
    fprintf(stdout, "Content-Type: text/html; charset=utf-8\n");
    // set cookie
    fprintf(stdout, "Set-Cookie: ");
    for (const auto& [key, value] : this->outcookie){
        fprintf(stdout, "%s=%s; ", key.c_str(), value.c_str());
    }
    fprintf(stdout, "\n\n");
    // output recorded cout
    fprintf(stdout, "%s", OUT.str().c_str());
}
HTTP::HTTP(std::string query, std::string postdata){
    std::vector<std::string> out;
    std::stringstream getstrstm(query);
    std::stringstream poststrstm(postdata);
    auto key = std::string{};
    auto val = std::string{};

    while (std::getline(getstrstm, key, '=')) {
        std::getline(getstrstm, val, '&');
        key = escaping(key);
        val = escaping(val);
        this->getparams[key] = val;
    }
    while (std::getline(poststrstm, key, '=' )) {
        std::getline(poststrstm, val, '&');
        this->postparams[key] = val;
    }
return;
}
HTTP::HTTP(){
    std::stringstream getstrstm(getenv("QUERY_STRING"));
    std::string postdata;
    std::getline(std::cin, postdata, static_cast<char>(0));
    std::stringstream poststrstm(postdata);
    auto key = std::string{};
    auto val = std::string{};

    // parse GET params
    while (std::getline(getstrstm, key, '=' )) {
        std::getline(getstrstm, val, '&');
        this->getparams[key] = val;
    }
    // parse POST params
    while (std::getline(poststrstm, key, '=' )) {
        std::getline(poststrstm, val, '&');
        this->postparams[key] = val;
    }
    // parse cookie
    if (getenv("HTTP_COOKIE") != nullptr){
        std::stringstream cookiestrstm(getenv("HTTP_COOKIE"));
        while (std::getline(cookiestrstm, key, '=')) {
            std::getline(cookiestrstm, val, ';');
            key = ltrim(&key);
            key = escaping(key);
            val = escaping(val);
            this->cookie[key] = val;
        }
    }
    return;
}
int HTTP::httpSendFile(std::string name){
    std::ifstream ifile;
    ifile.open(name, std::ios::in);
    if (!ifile){
        return -1;
    }
    std::string str;
    while (std::getline(ifile, str, '\n')) {
        std::cout << str << std::endl;
    }
    return 0;
}

HTTP::~HTTP(){
    return;
}

std::string HTTP::rawURLDecode(std::string str)
{
  std::string res = "";
  //cout << str.length () << endl;
  for (int i = 0; i < str.length (); i++)
    {

      if (str[i] != '%')
	{
	  res.append (1, str[i]);
	  //cout <<"0: " << res.back()<<endl;
	}
      if (str[i] == '%')
	{
	  i++; //skip '%'
	  //G cout <<": " << hex << CCtoI(str[i],str[i + 1]) << endl;
	  if (CCtoI(str[i],str[i + 1]) < 127)
	    {
	      res.append(1, char(CCtoI(str[i],str[i + 1])));
	      //cout <<"1: " << res.back()<<endl;
	      i++; //skip one hex (other hex will be skiped by i++ in for)
	      continue;
	    }
	    //cout <<"t "<<(CCtoI(str[i],str[i + 1])&0b11100000)<<"\n";
	  if ((CCtoI(str[i],str[i + 1])&0b11100000) == 0b11000000)
	    {
	      
	      res.append(1, char(CCtoI(str[i],str[i + 1])));
	      i = i+3; //skip (XX%): 2 hex numbers and '%' 
	      res.append(1, char(CCtoI(str[i],str[i + 1])));
	      i++; //skip one hex (other hex will be skiped by i++ in for)
	      continue;
	    }
	}
    }
  return res;
}

unsigned int HTTP::CtoI(char ch){
    if(ch >= 'A'){
	      return int(ch - 'A' + 10);
	  }
	  else{
	      return int(ch - '0');
	  }
}

unsigned int HTTP::CCtoI(char ch1, char ch2){
    return ((CtoI(ch1)<<4) + CtoI(ch2));
}


#endif
