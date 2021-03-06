#include "common/log.h"
#include "common/option.h"

#include "message/server_messenger.h"

#include "osd/osd.h"

int main(int argc, char *argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Usage: %s [port]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int port = atoi(argv[1]);

    int32_t log_prefix = 201;
    cceph_log_initial_id(log_prefix);

    int64_t log_id = cceph_log_new_id();
    int msg_work_thread_count = g_cceph_option.osd_msg_workthread_count;
    cceph_messenger* msger = cceph_messenger_new(&cceph_osd_process_message, NULL, msg_work_thread_count, log_id);
    cceph_server_messenger *smsger = new_cceph_server_messenger(msger, port, log_id);

    cceph_server_messenger_start(smsger, log_id);

    return 0;
}

