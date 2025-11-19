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
    void post_user(std::string name, bool admin);
    void post_msg(std::string user_name, std::string message);        // TODO:()        
    void post_directory(std::string user_name, std::string file_name, std::string file_data);      // TODO:()
    pqxx::result get_user(std::string user_name);
    pqxx::result get_user_list();
    pqxx::result get_msg_usr(std::string user_name);        // TODO:()
    pqxx::result get_msg_log();     // TODO:()
    pqxx::result get_directory(std::string file_name);      // TODO:()
    pqxx::result get_directory_list();      // TODO:()

    std::string conn_string;
    pqxx::connection conn;
    int table_count;

private:

};


#endif //CHATSHARE_DBENGINE_H