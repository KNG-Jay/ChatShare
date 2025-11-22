//
// Created by kng on 11/9/25.
//

#include "../server/include/Server.h"
#include <gtest/gtest.h>


TEST(AccountTests, InitializeAccount) {
    Account account("test", "test");
    ASSERT_TRUE(account.login_timestamp > 1);
}

TEST(ServerTests, StartServer) {

}