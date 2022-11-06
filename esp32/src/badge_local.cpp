/* Self header */
#include "badge_local.h"

/* Arduino libraries */
#include <Arduino.h>

/* Global instance */
class badge_local badge_local;

/**
 *
 */
int badge_local::setup(void) {

    /* Setup serial to badge */
    Serial2.begin(9600, SERIAL_8N1, CONFIG_BADGE_LOCAL_RX_PIN, CONFIG_BADGE_LOCAL_TX_PIN);

    /* Return success */
    return 0;
}

/**
 *
 */
int badge_local::task(void) {
    if (Serial2.available() >= 3) {
        struct badge_message msg;

        /* Extract first byte */
        uint8_t byte1 = Serial2.read();
        if ((byte1 >> 4) != 15) {
            Serial.printf(" [w] Received invalid first byte 0x%02X!\r\n", byte1);
            return 0;
        }
        msg.type = (enum badge_message_type)(byte1 & 0xF);

        /* Extract second byte */
        uint8_t byte2 = Serial2.read();
        msg.val1 = byte2 >> 4;
        msg.val2 = byte2 & 0xF;

        /* Extract third byte */
        uint8_t byte3 = Serial2.read();
        msg.val3 = byte3 >> 4;
        if ((byte3 & 0xF) != 14) {
            Serial.printf(" [w] Received invalid third byte 0x%02X!\r\n", byte3);
            return 0;
        }

        /* Add message to queue */
        for (int i = 0, j = m_queue_rx_index; i < CONFIG_BADGE_MESSAGE_QUEUE_SIZE; i++, j = (j + 1) % CONFIG_BADGE_MESSAGE_QUEUE_SIZE) {
            if (m_queue_rx[j].pending == false) {
                m_queue_rx[j].msg = msg;
                m_queue_rx[j].pending = true;

                /* Return message received */
                return 1;
            }
        }

        /* Return error */
        Serial.printf(" [e] No space in the queue to receive message!\r\n");
        return -1;
    }

    /* Return success */
    return 0;
}

/**
 *
 */
int badge_local::message_available(void) {
    int ret = 0;
    for (int i = 0; i < CONFIG_BADGE_MESSAGE_QUEUE_SIZE; i++) {
        if (m_queue_rx[i].pending == true) {
            ret++;
        }
    }
    return ret;
}

/**
 *
 */
int badge_local::message_clear(void) {
    for (int i = 0; i < CONFIG_BADGE_MESSAGE_QUEUE_SIZE; i++) {
        m_queue_rx[i].pending = false;
    }
    return 0;
}

/**
 *
 */
int badge_local::message_read(struct badge_message& msg) {
    if (m_queue_rx[m_queue_rx_index].pending == true) {
        msg = m_queue_rx[m_queue_rx_index].msg;
        m_queue_rx[m_queue_rx_index].pending = false;
        m_queue_rx_index = (m_queue_rx_index + 1) % CONFIG_BADGE_MESSAGE_QUEUE_SIZE;
        return 1;
    } else {
        return 0;
    }
}

/**
 *
 */
int badge_local::message_write(struct badge_message msg) {
    int res;

    /* Ensure data is valid */
    if ((msg.type & 0xF0) ||
        (msg.val1 & 0xF0) ||
        (msg.val2 & 0xF0) ||
        (msg.val3 & 0xF0)) {
        Serial.printf(" [e] Mesage payload exceeds 4 bits!\r\n");
        return -EINVAL;
    }

    /* Build message */
    uint8_t data[3] = {0};
    data[0] = (15 << 4) | (msg.type);
    data[1] = (msg.val1 << 4) | (msg.val2);
    data[2] = (msg.val3 << 4) | (14);

    /* Send it */
    res = Serial2.write(data, 3);
    if (res < 3) {
        Serial.printf(" [e] Error while sending message!\r\n");
        return -EIO;
    }

    /* Return success */
    return 0;
}
