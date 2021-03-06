#ifndef CCEPH_OS_TRANSACTION_H
#define CCEPH_OS_TRANSACTION_H

#include "common/types.h"
#include "os/coll_id.h"
#include "os/op.h"

#define CCEPH_OS_TRAN_OP_LIST_SIZE 8

typedef struct {
    int64_t             log_id;
    cceph_os_op_t       op;
    cceph_os_coll_id_t  cid;
    const char*         oid;
    int64_t             offset;
    int64_t             length;
    const char*         data;
} cceph_os_transaction_op;

typedef struct {
    cceph_os_transaction_op *op_buffer;
    int32_t                  op_buffer_length;
    int32_t                  op_buffer_index;
} cceph_os_transaction;

extern cceph_os_transaction* cceph_os_transaction_new();

extern int cceph_os_write(cceph_os_transaction* tran,
        cceph_os_coll_id_t  cid,
        const char*         oid,
        int64_t             offset,
        int64_t             length,
        const char*         data,
        int64_t             log_id);

extern int cceph_os_tran_get_op_count(
        cceph_os_transaction* tran,
        int64_t               log_id);

extern cceph_os_transaction_op* cceph_os_tran_get_op(
        cceph_os_transaction* tran,
        int32_t               index,
        int64_t               log_id);

#endif
