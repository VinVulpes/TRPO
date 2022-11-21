#ifndef __DB__CPP__CLASS__
#define __DB__CPP__CLASS__
#include "db.hpp"
#include <fstream>
template<typename KeyType, typename ValType>
ValType FILEDB<KeyType, ValType>::read_one(KeyType key){
    std::ifstream ifile(this->dbfilename);
    std::string ikey, ival;
    while (std::getline( ifile, ikey, '=' )) {
        std::getline( ifile, ival);
        if (std::string(key) == ikey){
            return ValType(ival);
        }
    }
    return "";
}
template<typename KeyType, typename ValType>
std::map<KeyType,ValType>* FILEDB<KeyType, ValType>::read_all(){
    std::map<KeyType,ValType> *res = new std::map<KeyType,ValType>;
    std::ifstream ifile(this->dbfilename);
    std::string ikey, ival;
    while (std::getline( ifile, ikey, '=' )) {
        std::getline( ifile, ival);
        (*res)[ikey]=ival;
    }
    return res;
}
template<typename KeyType, typename ValType>
FILEDB<KeyType, ValType>::FILEDB(){
    this->dbfilename = "db.txt";
}
template<typename KeyType, typename ValType>
FILEDB<KeyType, ValType>::~FILEDB(){
    return;
}

#endif