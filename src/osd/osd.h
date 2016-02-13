#ifndef CCEPH_OSD_OSD_H
#define CCEPH_OSD_OSD_H

#include "message/messenger.h"
#include "message/msg_header.h"

extern int osd_process_message(msg_handle* msg_handle, conn_id_t conn_id, msg_header* message, void* context);

#endif