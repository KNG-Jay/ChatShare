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
    ASSERT_EQ(engine.table_count, 4);
}
