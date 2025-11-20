//
// Created by kng on 11/9/25.
//

#ifndef CHATSHARE_DBENGINE_H
#define CHATSHARE_DBENGINE_H


#include <pqxx/pqxx>
#include <string>


class DBEngine {
public:
    DBEngine();
    ~DBEngine();

    void create_connection();
    void drop_connection();
    void check_database();
    void post_user(std::string user_name, std::string password, bool admin, std::string created_by);
    void post_msg(std::string user_name, std::string message);           
    void post_directory(std::string user_name, std::string file_name, std::string file_data);
    pqxx::result get_user(std::string user_name);
    pqxx::result get_user_list();
    pqxx::result get_msg_usr(std::string user_name);        
    pqxx::result get_msg_log();     
    pqxx::result get_directory(std::string file_name);      
    pqxx::result get_directory_list();      

    std::string conn_string;
    pqxx::connection conn;
    int table_count;

private:

};


std::time_t get_timestamp();


#endif //CHATSHARE_DBENGINE_H