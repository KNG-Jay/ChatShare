//
// Created by kng on 11/9/25.
//

#ifndef CHATSHARE_SERVER_H
#define CHATSHARE_SERVER_H

#include "../db-engine/include/DBEngine.h"
#include <boost/asio.hpp>
#include <Poco/Poco.h>
#include <string>


class Account {
public:
    Account(const std::string& user_name, const std::string& password);
    ~Account();

    int login_timestamp;

    void set_password(std::string new_password);    // TODO
    void set_admin(bool admin_status);      // TODO
    int get_id();   // TODO
    std::string get_username();     // TODO
    std::string get_password();     // TODO
    bool get_admin();       // TODO
    std::string get_creation_info();    // TODO
    std::string get_modified_info();    // TODO

private:
    int id;
    std::string user_name;
    std::string password;
    bool admin;
    std::string created;
    std::string created_by;
    std::string modified;
    std::string modified_by;

};

class Server {
public:
    Server();
    ~Server();

    std::unordered_map<std::string, std::string> users_online;     // <user_name, time_online>

    void startup();
    void shutdown();
    Account login(const std::string& user_name, const std::string& password);
    void logout(const std::string& user_name);

private:

};


#endif //CHATSHARE_SERVER_H