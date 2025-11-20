//
// Created by kng on 11/9/25.
//

#include "../db-engine/include/DBEngine.h"
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

TEST(DBEngineTests, UserDataTransaction) {     // TODO: ( get_user(std::string user_name) || get_user_list() )
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
    ASSERT_EQ(num_columns, 7);

    res = engine.get_msg_usr(user_name);
    ASSERT_TRUE(res.size() > 0);

    res = engine.get_msg_log();
    ASSERT_TRUE(res.size() > 0);
}

TEST(DBEngineTests, FileDataTransaction) {      // TODO: ( post_directory(std::string user_name, std::string file_name, std::string file_data) || get_directory(std::string file_name) || get_directory_list() )
    DBEngine engine;
    engine.create_connection();
    engine.check_database();
    
}

TEST(DBEngineTests, TimestampFunctional) {
    std::time_t utc_stamp = get_timestamp();
     
    std::println("UTC Timestamp Received: {}", utc_stamp);
    ASSERT_TRUE(utc_stamp > 0);
}