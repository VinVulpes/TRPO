#ifndef __HTTP_HPP_CLASS__
#define __HTTP_HPP_CLASS__
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <map>
class HTTP
{
    public:
    std::map<std::string,std::string> getparams;
    std::map<std::string,std::string> postparams;
    std::map<std::string,std::string> cookie;
    std::map<std::string,std::string> outcookie;
	private:
	std::stringstream OUT;

	public:
	HTTP();
	//Redirecting cout stream to string stream
	void init();
	//Send headers, cookie and user set data
	void send();
    HTTP(std::string query, std::string postdata);

	/*Send data from file to cout stream
	return 0: OK; -1: failure*/
	int httpSendFile(std::string name);
	// Get GET request parameters
	std::string httpGet(std::string name);
	// Get POST request parameters
	std::string httpPost(std::string name);
	std::string getCookie(std::string name);
	std::string setCookie(std::string name, std::string value);
	~HTTP();
};
#endif