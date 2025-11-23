//
// Created by kng on 11/9/25.
//

#include "../include/Server.h"
#include "pqxx/internal/statement_parameters.hxx"
#include <algorithm>
#include <boost/system/detail/error_code.hpp>
#include <exception>
#include <map>
#include <memory>
#include <print>
#include <sstream>
#include <string>

using boost::asio::ip::tcp;


Account::Account(const std::string& user_name, const std::string& password) :
    user_name(user_name), password(password) 
    {
        std::map<std::string, std::string> field_values;
        try {
            DBEngine engine;
            pqxx::result res = engine.get_user(user_name);
            for (auto const &field: res[0]) field_values.insert_or_assign(field.name(), field.c_str());
            if (password != field_values.at("password")) {
                throw("Password Was Incorrect!");
            } else {
                this->id = std::stoi(field_values.at("id"));
                this->user_name = field_values.at("user_name");
                this->password = field_values.at("password");
                this->admin = (field_values.at("admin") != "false");
                this->created = field_values.at("created");
                this->created_by = field_values.at("created_by");
                this->modified = field_values.at("modified");
                this->modified_by = field_values.at("modified_by");
                this->login_timestamp = get_timestamp();
            }
        } catch(std::exception ex) {
            std::println("[ERROR] Failed To Populate Account: {}", ex.what());
            throw;
        }
}
Account::~Account() {}

void Account::set_password(std::string new_password) 
{
    this->password = new_password;
}

void Account::set_admin(bool admin_status) 
{
    this->admin = admin_status;
}

int Account::get_id() 
{
    return this->id;
}

std::string Account::get_username() 
{
    return this->user_name;
}

std::string Account::get_password() 
{
    return this->password;
}

bool Account::get_admin() 
{
    return this->admin;
}

std::string Account::get_creation_info() 
{
    std::string info_str("Created By: " + this->created_by + " On: " + this->created);
    return info_str;
}

std::string Account::get_modified_info() 
{
    std::string info_str("Modified By: " + this->modified_by + "On: " + this->modified);
    return info_str;
}


Server::Server() :
    acceptor_(this->io, tcp::endpoint(tcp::v4(), port = 8081))
    {
    // TODO: ( DEFAULT SERVER STARTUP IMPLEMENTATION )
    do_accept();
}
Server::~Server()
{
    // TODO: ( DEFAULT SERVER SHUTDOWN IMPLEMENTATION )
}

Account Server::login(const std::string& user_name, const std::string& password) 
{
    Account account(user_name, password);
    // TODO: ( Proccess And Add User To users_online )
    
    this->users_online.insert_or_assign(
        account.get_username(), std::to_string(account.login_timestamp));
    return account;
}

void Server::logout(Account account) 
{
    try {
        std::println("Trying To Logout...");
        if (this->users_online.contains(account.get_username())) {
            // TODO: ( Proccess And Remove User To users_online )

            this->users_online.at(account.get_username()).erase();
            std::println("Successfully Logged Out Of Server!");
        } else
            throw("[ERROR] Failed To Find user In users_online!");
    } catch(std::exception ex) {
        std::println("[ERROR] Failed To Logout!: {}", ex.what());
        throw;
    }
}

void Server::do_accept()
{
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket) {
            if (!ec) {
                //std::make_shared<Session>(std::move(socket))->start();
            }
            do_accept();
        }
    );
}

