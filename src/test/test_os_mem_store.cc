extern "C" {
#include "common/errno.h"
#include "os/mem_store.h"
}

#include "gtest/gtest.h"

TEST(os_mem_store, cceph_mem_store_coll_node) {
    cceph_rb_root root = CCEPH_RB_ROOT;
    for (int i = 0; i < 1000; i++) {
        cceph_mem_store_coll_node *node = new cceph_mem_store_coll_node();
        node->cid = i;
        int ret = TEST_cceph_mem_store_coll_node_insert(&root, node);
        EXPECT_EQ(CCEPH_OK, ret);
    }
    for (int i = 0; i < 1000; i++) {
        cceph_mem_store_coll_node *node = TEST_cceph_mem_store_coll_node_search(&root, i);
        EXPECT_NE((cceph_mem_store_coll_node*)NULL, node);
        EXPECT_EQ(i, node->cid);
    }
}

TEST(os_mem_store, cceph_mem_store_object_node) {
    cceph_rb_root root = CCEPH_RB_ROOT;
    for (int i = 0; i < 1000; i++) {
        cceph_mem_store_object_node *node = new cceph_mem_store_object_node();
        node->oid = (char*)malloc(sizeof(char) * 256);
        bzero(node->oid, 256);
        sprintf(node->oid, "%d", i);
        int ret = TEST_cceph_mem_store_object_node_insert(&root, node);
        EXPECT_EQ(CCEPH_OK, ret);
    }

    char key[256];
    for (int i = 0; i < 1000; i++) {
        bzero(key, 256);
        sprintf(key, "%d", i);
        cceph_mem_store_object_node *node = TEST_cceph_mem_store_object_node_search(&root, key);
        EXPECT_NE((cceph_mem_store_object_node*)NULL, node);
        EXPECT_STREQ(key, node->oid);
    }
}

TEST(os_mem_store, cceph_mem_store_new) {
    cceph_mem_store *store = cceph_mem_store_new();
    EXPECT_NE((cceph_mem_store*)NULL, store);
}

