//
// Created by kng on 11/9/25.
//

#include "../db-engine/include/DBEngine.h"
#include <gtest/gtest.h>

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

TEST(DBEngineTests, UserDataTransaction) {     // TODO: ( post_user(std::string name, bool admin) || get_user(std::string user_name) || get_user_list() )

}

TEST(DBEngineTests, ChatDataTransaction) {      // TODO: ( post_msg(std::string user_name, std::string message) || get_msg_usr(std::string user_name) || get_msg_log() )

}

TEST(DBEngineTests, FileDataTransaction) {      // TODO: ( post_directory(std::string user_name, std::string file_name, std::string file_data) || get_directory(std::string file_name) || get_directory_list() )

}