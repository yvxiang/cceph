#ifndef CCEPH_LOG_H
#define CCEPH_LOG_H

#define CCEPH_STATUS_MESSAGE_LENGTH 4096

struct status {
    int result;
    char message[CCEPH_STATUS_MESSAGE_LENGTH];
};

#endif