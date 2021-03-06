extern "C" {
#include "common/log.h"
}

#include "gtest/gtest.h"

TEST(common_log, log_id) {
    int prefix = 1;
    cceph_log_initial_id(prefix);

    int64_t log_id_1 = cceph_log_new_id();
    EXPECT_TRUE(log_id_1 >= 10000000000);

    int64_t log_id_2 = cceph_log_new_id();
    EXPECT_EQ(log_id_1 + 1, log_id_2);
}
