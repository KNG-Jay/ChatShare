//
// Created by kng on 11/9/25.
//

#include "../db-engine/include/DBEngine.h"
#include "pqxx/internal/statement_parameters.hxx"
#include <cstddef>
#include <gtest/gtest.h>
#include <print>
#include <string>

TEST(DBEngineTests, CreateConnection) {
    DBEngine engine;

    engine.create_connection();
    ASSERT_TRUE(engine.conn.is_open());

    engine.drop_connection();
    ASSERT_FALSE(engine.conn.is_open());
}

TEST(DBEngineTests, CheckDBIntegrity) {
    DBEngine engine;
    engine.create_connection();

    engine.check_database();
    ASSERT_EQ(engine.table_count, 3);
}

TEST(DBEngineTests, UserDataTransaction) {
    DBEngine engine;
    engine.create_connection();
    engine.check_database();

    std::string user_name = "test";
    std::string password = "test";
    bool admin = false;
    std::string created_by = "system";
    engine.post_user(user_name, password, admin, created_by);

    pqxx::work trx{engine.conn};
    pqxx::result res = trx.exec("SELECT * FROM user_data LIMIT 0;");
    std::size_t num_columns = res.columns();
    trx.abort();
    ASSERT_EQ(num_columns, 8);

    res = engine.get_user(user_name);
    ASSERT_EQ(res.size(), 1);

    res = engine.get_user_list();
    ASSERT_EQ(res.size(), 1);
}

TEST(DBEngineTests, ChatDataTransaction) {
    DBEngine engine;
    engine.create_connection();
    engine.check_database();

    std::string user_name = "system";
    std::string message = "test message";
    engine.post_msg(user_name, message);

    pqxx::work trx{engine.conn};
    pqxx::result res = trx.exec("SELECT * FROM chat_log LIMIT 0;");
    std::size_t num_columns = res.columns();
    trx.abort();
    ASSERT_EQ(num_columns, 4);

    res = engine.get_msg_usr(user_name);
    ASSERT_TRUE(res.size() > 0);

    res = engine.get_msg_log();
    ASSERT_TRUE(res.size() > 0);
}

TEST(DBEngineTests, FileDataTransaction) {      
    DBEngine engine;
    engine.create_connection();
    engine.check_database();
    
    std::string user_name = "system";
    std::string file_location = "vcpkg.json";
    engine.post_directory(user_name, file_location);

    pqxx::work trx{engine.conn};
    pqxx::result res = trx.exec("SELECT * FROM file_metadata LIMIT 0;");
    std::size_t num_columns = res.columns();
    trx.abort();
    ASSERT_EQ(num_columns, 6);

    std::filesystem::path file_path = file_location;
    res = engine.get_directory(file_path.stem());
    ASSERT_TRUE(res.size() > 0);

    res = engine.get_directory_list();
    ASSERT_TRUE(res.size() > 0);
}

TEST(DBEngineTests, TimestampFunctional) {
    std::time_t utc_stamp = get_timestamp();
     
    std::println("UTC Timestamp Received: {}", utc_stamp);
    ASSERT_TRUE(utc_stamp > 0);
}