#ifndef BADGE_MESSAGE_H
#define BADGE_MESSAGE_H

/* Config */
#include "../cfg/config.h"

/* C/C++ libraries */
#include <errno.h>
#include <stdint.h>

/* */
enum badge_message_type {
    BADGE_MESSAGE_TYPE_POSITION_A = 0b0001,
    BADGE_MESSAGE_TYPE_POSITION_B = 0b0010,
    BADGE_MESSAGE_TYPE_POSITION_BALL = 0b0011,
    BADGE_MESSAGE_TYPE_IAM = 4,
    BADGE_MESSAGE_TYPE_QUIT = 5,
};

/* Struct describing messages sent accross badges */
struct badge_message {
    enum badge_message_type type;
    uint8_t val1;
    uint8_t val2;
    uint8_t val3;
};

#endif