//
// Created by kng on 11/9/25.
//

#ifndef CHATSHARE_SERVER_H
#define CHATSHARE_SERVER_H


#include "../../db-engine/include/DBEngine.h"
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <rfl/json.hpp>
#include <rfl.hpp>
#include <boost/asio.hpp>
#include <Poco/Poco.h>
#include <Poco/File.h>
#include <Poco/Path.h>
#include <string>


class Account 
{
public:
    Account(const std::string& user_name, const std::string& password);
    ~Account();

    int login_timestamp;

    void set_password(const std::string& new_password);    
    void set_admin(const bool& admin_status);      
    int get_id();   
    std::string get_username();     
    std::string get_password();     
    bool get_admin();       
    std::string get_creation_info();    
    std::string get_modified_info();    

private:
    int id_;
    std::string user_name_;
    std::string password_;
    bool admin_;
    std::string created_;
    std::string created_by_;
    std::string modified_;
    std::string modified_by_;

};


class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(boost::asio::ip::tcp::socket& socket);
    ~Session();

    void start();

private:
    boost::asio::ip::tcp::socket socket_;
    char data_[1024];

    void do_read();

};


class Server 
{
public:
    Server();
    ~Server();

    std::unordered_map<std::string, std::string> users_online;     // <user_name, time_online>
    boost::asio::io_context io;
    short port;

    void startup();
    void shutdown();
    Account login(const std::string& user_name, const std::string& password);
    void logout(Account account);
    void do_accept();

private:
    boost::asio::ip::tcp::acceptor acceptor_;

};


struct Message {
    int id;
    std::string user_name;
    std::string message;
    std::string created;
};

struct File {
    int id;
    std::string user_name;
    std::string file_name;
    std::string file_extension;
    std::string binary_data;
    std::string created;
};


#endif //CHATSHARE_SERVER_H