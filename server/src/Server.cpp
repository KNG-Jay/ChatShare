//
// Created by kng on 11/9/25.
//

#include "../include/Server.h"
#include "DBEngine.h"
#include "pqxx/internal/statement_parameters.hxx"
#include <exception>
#include <print>
#include <string>


Account::Account(const std::string& user_name, const std::string& password) :
    user_name(user_name), password(password) {
        try {
            DBEngine engine;
            pqxx::result res = engine.get_user(user_name);
            // TODO: ( Proccess And Assign login_timestamp )
        } catch(std::exception ex) {
            std::println("[ERROR] Failed To Populate Account: {}", ex.what());
            throw;
        }
    }
Account::~Account() {}


Server::Server() {
    // TODO: ( DEFAULT SERVER STARTUP IMPLEMENTATION )
}
Server::~Server() {
    // TODO: ( DEFAULT SERVER SHUTDOWN IMPLEMENTATION )
}

Account Server::login(const std::string& user_name, const std::string& password) {
    // TODO: ( Proccess And Add User To users_online )
}

void Server::logout(const std::string& user_name) {
    // TODO: ( Proccess And Remove User To users_online )
}

