//
// Created by kng on 11/9/25.
//

#ifndef CHATSHARE_DBENGINE_H
#define CHATSHARE_DBENGINE_H


#include <pqxx/pqxx>
#include <string>

// TODO: ( Incorporate UPDATE And DELETE Functionality )
class DBEngine {
public:
    DBEngine();
    ~DBEngine();

    std::string conn_string;
    pqxx::connection conn;
    int table_count;

    void create_connection();
    void drop_connection();
    void check_database();
    void post_user(const std::string user_name, const std::string password, const bool admin, const std::string created_by);
    void post_msg(const std::string user_name, const std::string message);           
    void post_directory(const std::string user_name, const std::string& file_location);
    pqxx::result get_user(std::string user_name);
    pqxx::result get_user_list();
    pqxx::result get_msg_usr(std::string user_name);        
    pqxx::result get_msg_log();     
    pqxx::result get_directory(std::string file_name);      
    pqxx::result get_directory_list();      

private:

};


std::time_t get_timestamp();


#endif //CHATSHARE_DBENGINE_H