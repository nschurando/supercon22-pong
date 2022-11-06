/* Self header */
#include "game.h"

/* Project code */
#include "badge_local.h"
#include "badge_message.h"
#include "badge_remote.h"

/* Arduino libraries */
#include <Arduino.h>

/* */
class game game;

/**
 *
 */
int game::task(void) {
    int res;

#if 0
    /* State machine */
    switch (m_sm) {

        case STATE_0: {

            /* Read message from badge */
            struct badge_message msg;
            res = badge_local.message_read(msg);
            if (res < 0) {
                return -EIO;
            } else if (res == 0) {
                break;
            }

            /* Wait for indication of whether we are A or B */
            if (msg.type == BADGE_MESSAGE_TYPE_IAM) {
				// Do something ?
				m_sm 
            }
            break;
        }

        default: {
            Serial.printf(" [e] Invalid state!");
            m_sm = STATE_0;
            return -1;
        }
    }
#endif

    struct badge_message msg;

    /* Read any message from local badge and forward it to remote badge */
    res = badge_local.message_read(msg);
    if (res < 0) {
        return -EIO;
    } else if (res > 0) {
        res = badge_remote.message_write(msg);
        if (res < 0) {
            Serial.printf(" [e] Failed to forward local message to remote!");
            // TODO : Warn local badge
            return -EIO;
        }
    }

    /* Read any message from remote badge and forward it to local badge */
    res = badge_remote.message_read(msg);
    if (res < 0) {
        return -EIO;
    } else if (res > 0) {
        res = badge_local.message_write(msg);
        if (res < 0) {
            Serial.printf(" [e] Failed to forward remote message to local!");
            // TODO : Warn remote badge
            return -EIO;
        }
    }

    /* Return succes */
    return 0;
}
