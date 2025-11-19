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
    void post_msg();
    void post_directory();
    void post_user();
    void get_msg_log();
    void get_directory();
    void get_directory_list();
    void get_user();

    std::string conn_string;
    pqxx::connection conn;

private:

};


#endif //CHATSHARE_DBENGINE_H