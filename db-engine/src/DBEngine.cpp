//
// Created by kng on 11/9/25.
//

#include "../include/DBEngine.h"
#include <iostream>
#include <exception>
#include <print>


DBEngine::DBEngine() :
    conn_string("host=localhost port=5432 user=kng password=Recon777 dbname=chatshare connect_timeout=15") {}
    
DBEngine::~DBEngine() {}


void DBEngine::create_connection() {
    try {
        std::println("Trying To Connect To The Database...");
        conn = pqxx::connection(conn_string);
        if (conn.is_open()) std::println("Database Connection Active!");
        else throw pqxx::broken_connection("Connection Failed To Connect And/Or Remain Open");
    } catch(pqxx::broken_connection ex) {
        std::println("[ERROR] Failed To Connect To Database: %s", ex.what());
    }
}

void DBEngine::drop_connection() {
    try {
        std::println("Trying To Close Connection...");
        conn.close();
        std::println("Connection Has Been Closed");
    } catch(std::exception ex) {
        std::println("[ERROR] Failed To Drop Connection: %s", ex.what());
    }
}

void DBEngine::check_database() {

}