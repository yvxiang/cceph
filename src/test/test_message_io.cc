extern "C" {
#include "include/errno.h"
#include "message/io.h"
}

#include <errno.h>

#include "bhook.h"
#include "gtest/gtest.h"

char* sys_func_name_recv = (char*)"recv";
char* lib_func_name_recv_from_conn = (char*)"recv_from_conn";

ssize_t MOCK_recv_from_conn_recv_normal(int sockfd, void *buf, size_t len, int flags) {
    EXPECT_EQ(1, sockfd);
    EXPECT_TRUE(buf != NULL);
    EXPECT_EQ(16, len);
    EXPECT_EQ(0, flags);

    return len;
}
ssize_t MOCK_recv_from_conn_recv_closed(int sockfd, void *buf, size_t len, int flags) {
    EXPECT_EQ(1, sockfd);
    EXPECT_TRUE(buf != NULL);
    EXPECT_EQ(16, len);
    EXPECT_EQ(0, flags);

    errno = EAGAIN + 1;
    return -1;
}
ssize_t MOCK_recv_from_conn_recv_again(int sockfd, void *buf, size_t len, int flags) {
    static int call_time = -1;
    call_time++;

    EXPECT_EQ(1, sockfd);
    EXPECT_TRUE(buf != NULL);
    EXPECT_EQ(0, flags);

    if (call_time == 0) {
        EXPECT_EQ(16, len);
        return 4;
    } else if (call_time == 1) {
        EXPECT_EQ(16 - 4, len);
        errno = EAGAIN;
        return -1;
    } else if (call_time == 2) {
        EXPECT_EQ(16 - 4, len);
        return 12;
    }

    EXPECT_TRUE(0 == 1);
    return -1;
}
//static int recv_from_conn(int data_fd, void* buf, size_t size, int64_t log_id) {
TEST(message_io, recv_from_conn) {
    int data_fd = 1;
    char buf[16];
    size_t size = 16;
    int64_t log_id = 1;

    //Case: conn normal
    attach_and_enable_func(sys_func_name_recv, (void*)&MOCK_recv_from_conn_recv_normal);
    EXPECT_EQ(size, TEST_recv_from_conn(data_fd, buf, size, log_id));
    detach_func(sys_func_name_recv);

    //Case: conn closed
    attach_and_enable_func(sys_func_name_recv, (void*)&MOCK_recv_from_conn_recv_closed);
    EXPECT_EQ(CCEPH_ERR_CONN_CLOSED, TEST_recv_from_conn(data_fd, buf, size, log_id));
    detach_func(sys_func_name_recv);

    //Case: conn again
    attach_and_enable_func(sys_func_name_recv, (void*)&MOCK_recv_from_conn_recv_again);
    EXPECT_EQ(size, TEST_recv_from_conn(data_fd, buf, size, log_id));
    detach_func(sys_func_name_recv);
}

int MOCK_recv_int8_recv_from_conn(int data_fd, void* buf, size_t size, int64_t log_id) {
    EXPECT_EQ(sizeof(int8_t), size);
    EXPECT_TRUE(buf != NULL);
    EXPECT_EQ(122, log_id);

    int8_t value = 37;
    memcpy(buf, &value, size);

    if (data_fd == 1) return 1; 
    if (data_fd == 2) return 0; 
    if (data_fd == 3) return -2; 

    EXPECT_TRUE(0 == 1);
    return 0;
}
TEST(message_io, recv_int8) {
    attach_and_enable_func_lib(lib_func_name_recv_from_conn, (void*)&MOCK_recv_int8_recv_from_conn);

    //Case: normal
    int8_t value   = 0;
    int ret = recv_int8(1, &value, 122);
    EXPECT_EQ(0, ret);
    EXPECT_EQ(37, value);

    //Case: imcomplete read
    value  = 0;
    ret = recv_int8(2, &value, 122);
    EXPECT_EQ(-1, ret);

    //Case: err
    value  = 0;
    ret = recv_int8(3, &value, 122);
    EXPECT_EQ(-2, ret);

    detach_func(lib_func_name_recv_from_conn);
}