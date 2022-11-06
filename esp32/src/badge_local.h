#ifndef BADGE_LOCAL_H
#define BADGE_LOCAL_H

/* Config */
#include "../cfg/config.h"

/* Project code */
#include "badge_message.h"

/* C/C++ libraries */
#include <errno.h>
#include <stdint.h>

/* Class for exchanging messages with local badge */
class badge_local {
   public:
    int setup(void);
    int task(void);
    int message_available(void);
    int message_clear(void);
    int message_read(struct badge_message &msg);
    int message_write(struct badge_message msg);

   protected:
    struct {
        bool pending;
        struct badge_message msg;
    } m_queue_rx[CONFIG_BADGE_MESSAGE_QUEUE_SIZE] = {0};
    int m_queue_rx_index = 0;
};

/* Global instance */
extern class badge_local badge_local;

#endif
