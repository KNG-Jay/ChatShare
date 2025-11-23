//
// Created by kng on 11/9/25.
//

#include "../include/DBEngine.h"
#include <exception>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <print>
#include <chrono>
#include <iostream>
#include <string>
#include <vector>


DBEngine::DBEngine() :
    conn_string("host=localhost port=5432 user=kng password=Recon777 dbname=chatshare connect_timeout=15")
    {
        create_connection();
        check_database();
}
    
DBEngine::~DBEngine() {}


void DBEngine::create_connection() 
{
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

void DBEngine::drop_connection() 
{
    try {
        std::println("Trying To Close Connection...");
        this->conn.close();
        std::println("Connection Has Been Closed");
    } catch(std::exception ex) {
        std::println("[ERROR] Failed To Drop Connection: {}", ex.what());
    }
}

void DBEngine::check_database() 
{
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

    if (this->table_count < 3 || this->table_count > 4) {
        try { 
        pqxx::work trx{this->conn};
        trx.exec("\
            CREATE TABLE IF NOT EXISTS user_data ( \
            id SERIAL PRIMARY KEY, \
            user_name VARCHAR(100) UNIQUE, \
            password VARCHAR(100), \
            admin BOOLEAN, \
            created TIMESTAMP, \
            created_by VARCHAR(100), \
            modified TIMESTAMP, \
            modified_by VARCHAR(100) \
            );"
        );
        trx.exec("\
            CREATE TABLE IF NOT EXISTS chat_log ( \
            id SERIAL PRIMARY KEY, \
            user_name VARCHAR(100), \
            message TEXT, \
            created TIMESTAMP \
            );"
        );
        trx.exec("\
            CREATE TABLE IF NOT EXISTS file_metadata ( \
            id SERIAL PRIMARY KEY, \
            user_name VARCHAR(100), \
            file_name VARCHAR(255) UNIQUE, \
            file_extension CHAR(100), \
            file_data BYTEA UNIQUE, \
            created TIMESTAMP \
            );"
        );
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
// TODO: ( Add User Info Filter And Verification )
void DBEngine::post_user(std::string user_name, std::string password, bool admin, std::string created_by) 
{
    pqxx::result res;
    std::string db_name = "user_data";

    try {
        std::println("Trying To Add User To Database...");
        pqxx::work trx{this->conn};
        res = trx.exec("\
            INSERT INTO user_data \
            (user_name, password, admin, created, created_by) \
            VALUES ($1, $2, $3, to_timestamp($4), $5) \
            ON CONFLICT (user_name) DO NOTHING; \
            ",
            {user_name, password, admin, get_timestamp(), created_by}
        );
        trx.commit();
        std::println("Successfully Added User To Database!");
    } catch(std::exception ex) {
        std::println("[Error] Failed To Add User To Database: {}", ex.what());
        throw;
    }
}

void DBEngine::post_msg(std::string user_name, std::string message) 
{
    pqxx::result res;
    std::string db_name = "chat_log";

    try {
        std::println("Trying To Post Message To Database...");
        pqxx::work trx{this->conn};
        res = trx.exec(" \
            INSERT INTO chat_log \
            (user_name, message, created) \
            VALUES ($1, $2, to_timestamp($3)) \
            ",
            {user_name, message, get_timestamp()}
        );
        trx.commit();
        std::println("Successfully Added Message To Database!");
    } catch(std::exception ex) {
        std::println("[ERROR] Failed To Post Message To Database: {}", ex.what());
        throw;
    }
}

void DBEngine::post_directory(const std::string user_name, const std::string& file_location) 
{
    pqxx::result res;
    std::ifstream file;
    std::vector<char> buffer;

    try {
        file.open(file_location, std::ios::binary);
        if (!file.is_open()) throw("[ERROR] Could Not Open File!!!");
        else
            buffer.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        if (buffer.size() > 1000000000) throw("[ERROR] File Size To Large...\tCannot Be Larger Than 1GB!");
    } catch (std::exception ex) {
        std::println("[ERROR] Failed To Process File For Database Storage: {}", ex.what());
        throw;
    }

    std::filesystem::path file_path = file_location;
    std::string file_name = file_path.stem();
    std::string file_extension = file_path.extension();
    try {
        std::println("Trying To Post File To Database...");
        std::string binary_data(buffer.begin(), buffer.end());
        pqxx::work trx{this->conn};
        res = trx.exec(" \
            INSERT INTO file_metadata \
            (user_name, file_name, file_extension, file_data, created) \
            VALUES ($1, $2, $3, $4, to_timestamp($5)) \
            ON CONFLICT (file_name) DO NOTHING; \
            ",
            {user_name, file_name, file_extension, binary_data, get_timestamp()}
        );
        trx.commit();
        std::println("Successfully Added File To Database!");
    } catch (std::exception ex) {
        std::println("[ERROR] Failed To Post File To Database: {}", ex.what());
        throw;
    }
}
// TODO:( Messages Sent && Files Sent/Received )
pqxx::result DBEngine::get_user(std::string user_name) 
{
    pqxx::result res;

    try {
        std::println("Trying To Get User: {}", user_name);
        pqxx::work trx{this->conn};
        res = trx.exec("\
            SELECT * FROM user_data WHERE user_name=$1 \
            ", pqxx::params{user_name}
        );
        trx.commit();
        std::println("Successfully Retrieved Data For: {}", user_name);
        return res;
    } catch(std::exception ex) {
        std::println("[ERROR] Failed To Get User Data: {}", ex.what());
        throw;
    }
}

pqxx::result DBEngine::get_user_list() 
{
    pqxx::result res;

    try {
        std::println("Trying To Get User List...");
        pqxx::work trx{this->conn};
        res = trx.exec("\
            SELECT * FROM user_data; \
            "
        );
        trx.commit();
        std::println("Successfully Retrieved The User List!");
        return res;
    } catch(std::exception ex) {
        std::println("[ERROR] Failed To Get Users List: {}", ex.what());
        throw;
    }
}

pqxx::result DBEngine::get_msg_usr(std::string user_name) 
{
    pqxx::result res;

    try {
        std::println("Trying To Get Messages For User: {}", user_name);
        pqxx::work trx{this->conn};
        res = trx.exec("\
            SELECT * FROM chat_log WHERE user_name=$1 \
            ", pqxx::params{user_name}
        );
        trx.commit();
        std::println("Successfully Retrieved Messages For User: {}", user_name);
        return res;
    } catch(std::exception ex) {
        std::println("[ERROR] Failed To Get User Messages: {}", ex.what());
        throw;
    }
}

pqxx::result DBEngine::get_msg_log() 
{
    pqxx::result res;

    try {
        std::println("Trying To Get Messages...");
        pqxx::work trx{this->conn};
        res = trx.exec("\
            SELECT * FROM chat_log; \
            "
        );
        trx.commit();
        std::println("Successfully Retrieved All Messages!");
        return res;
    } catch(std::exception ex) {
        std::println("[ERROR] Failed To Get Messages: {}", ex.what());
        throw;
    }
}

pqxx::result DBEngine::get_directory(std::string file_name) 
{
    pqxx::result res;

    try {
        std::println("Trying To Get File: {}", file_name);
        pqxx::work trx{this->conn};
        res = trx.exec("\
            SELECT * FROM file_metadata WHERE file_name=$1 \
            ", pqxx::params{file_name}
        );
        trx.commit();
        std::println("Successfully Retrieved File: {}", file_name);
        return res;
    } catch(std::exception ex) {
        std::println("[ERROR] Failed To Get File: {}", ex.what());
        throw;
    }
}

pqxx::result DBEngine::get_directory_list() 
{
    pqxx::result res;

    try {
        std::println("Trying To Get Files...");
        pqxx::work trx{this->conn};
        res = trx.exec("\
            SELECT * FROM file_metadata; \
            "
        );
        trx.commit();
        std::println("Successfully Retrieved All Files!");
        return res;
    } catch(std::exception ex) {
        std::println("[ERROR] Failed To Get Files: {}", ex.what());
        throw;
    }
}


std::time_t get_timestamp() 
{
    std::println("Retrieving Timestamp In UTC...");
    std::chrono::utc_clock clock;

    try {
        const std::chrono::time_point now = clock.now();
        const auto time_since_epoch = now.time_since_epoch();
        const std::time_t utc_stamp =
            std::chrono::duration_cast<std::chrono::seconds>(time_since_epoch).count();
        return utc_stamp;
    } catch(std::exception ex) {
        std::println("[ERROR] Failed To Get Timestamp: {}", ex.what());
        throw;
    }
}