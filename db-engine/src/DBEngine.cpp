//
// Created by kng on 11/9/25.
//

#include "../include/DBEngine.h"
#include "pqxx/internal/statement_parameters.hxx"
#include <exception>
#include <print>


DBEngine::DBEngine() :
    conn_string("host=localhost port=5432 user=kng password=Recon777 dbname=chatshare connect_timeout=15") {}
    
DBEngine::~DBEngine() {}


void DBEngine::create_connection() {
    try {
        std::println("Trying To Connect To The Database...");
        this->conn = pqxx::connection(this->conn_string);
        if (this->conn.is_open()) std::println("Database Connection Active!");
        else throw pqxx::broken_connection("Connection Failed To Connect And/Or Remain Open");
    } catch(pqxx::broken_connection ex) {
        std::println("[ERROR] Failed To Connect To Database: {}", ex.what());
        throw;
    }
}

void DBEngine::drop_connection() {
    try {
        std::println("Trying To Close Connection...");
        this->conn.close();
        std::println("Connection Has Been Closed");
    } catch(std::exception ex) {
        std::println("[ERROR] Failed To Drop Connection: {}", ex.what());
    }
}

void DBEngine::check_database() {
    pqxx::result res;

    try {
        pqxx::work trx{this->conn};
        res = trx.exec("SELECT tablename FROM pg_tables WHERE schemaname = 'public'");
        this->table_count = res.size();
        std::println("[PRECHECK] Tables In Database: {}", this->table_count);
        res.clear();
    } catch(const std::exception& ex) {
        std::println("[ERROR] Failed To Check Database Tables: {}", ex.what());
        throw;
    }

    if (this->table_count < 3) {
        try { 
        pqxx::work trx{this->conn};
        trx.exec("\
            CREATE TABLE IF NOT EXISTS user_data ( \
            ID INT PRIMARY KEY, \
            user_name VARCHAR(50), \
            password VARCHAR(50), \
            admin BOOLEAN, \
            created TIMESTAMP, \
            modified TIMESTAMP \
            );");
        trx.exec("\
            CREATE TABLE IF NOT EXISTS chat_log ( \
            ID INT PRIMARY KEY, \
            user_name VARCHAR(50), \
            message TEXT, \
            created TIMESTAMP \
            );");
        trx.exec("\
            CREATE TABLE IF NOT EXISTS file_metadata ( \
            ID INT PRIMARY KEY, \
            user_name VARCHAR(50), \
            file_name VARCHAR(255), \
            file_extension CHAR(50), \
            file_data BYTEA, \
            created TIMESTAMP \
            );");
        trx.commit();
        } catch(const std::exception& ex) {
            std::println("[ERROR] FAILED TO CREATE TABLES: {}", ex.what());
            throw;
        }

        try {
            pqxx::work trx{this->conn};
            res = trx.exec("SELECT tablename FROM pg_tables WHERE schemaname = 'public'");
            this->table_count = res.size();
            std::println("[POSTCHECK] Tables In Database: {}", this->table_count);
            res.clear();
        } catch(const std::exception& ex) {
            std::println("[ERROR] FAILED TO CREATE TABLES: {}", ex.what());
            throw;
        }
    }
}

void DBEngine::post_user(std::string name, bool admin) {

}

pqxx::result DBEngine::get_user(std::string user_name) {     // TODO:( Messages Sent && Files Sent/Received )

}

pqxx::result DBEngine::get_user_list() {

}