//
// Created by kng on 11/9/25.
//

#ifndef CHATSHARE_DBENGINE_H
#define CHATSHARE_DBENGINE_H


#include <iostream>
#include <pqxx/pqxx>


class DBEngine {
public:
    DBEngine();
    ~DBEngine();

private:
    pqxx::connection conn;

};


#endif //CHATSHARE_DBENGINE_H