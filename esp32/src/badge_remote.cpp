/* Self header */
#include "badge_remote.h"

/* Global instance */
class badge_remote badge_remote;

/**
 *
 */

void bytes_received(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
    Serial.printf(" [d] Received %d bytes of message from remote badge.", data_len);

    if (data_len >= 3) {
        struct badge_message msg;

        /* Extract first byte */
        uint8_t byte1 = data[0];
        if ((byte1 >> 4) != 15) {
            Serial.printf(" [w] Received invalid first byte 0x%02X!\r\n", byte1);
            return;
        }
        msg.type = (enum badge_message_type)(byte1 & 0xF);

        /* Extract second byte */
        uint8_t byte2 = data[1];
        msg.val1 = byte2 >> 4;
        msg.val2 = byte2 & 0xF;

        /* Extract third byte */
        uint8_t byte3 = data[2];
        msg.val3 = byte3 >> 4;
        if ((byte3 & 0xF) != 14) {
            Serial.printf(" [w] Received invalid third byte 0x%02X!\r\n", byte3);
            return;
        }

        /* Add message to queue */
        for (int i = 0, j = badge_remote.m_queue_rx_index; i < CONFIG_BADGE_MESSAGE_QUEUE_SIZE; i++, j = (j + 1) % CONFIG_BADGE_MESSAGE_QUEUE_SIZE) {
            if (badge_remote.m_queue_rx[j].pending == false) {
                badge_remote.m_queue_rx[j].msg = msg;
                badge_remote.m_queue_rx[j].pending = true;

                /* Return message received */
                return;
            }
        }

        /* Return error */
        Serial.printf(" [e] No space in the queue to receive message!\r\n");
        return;
    }
}

// callback when data is sent
void data_sent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.print(" [d] Last Packet Send Status:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

/**
 *
 */
int badge_remote::setup(void) {

    /* Setup esp now */
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return -1;
    }

    /* Register callback */
    esp_now_register_recv_cb(bytes_received);
    esp_now_register_send_cb(data_sent);

    /* Determine mac address of other badge
     * Nico = 24:D7:EB:41:7B:1C
     * GJ = ? */
#if 0
    String mac_address = WiFi.macAddress();
    if (mac_address == "24:D7:EB:41:7B:1C") {
        m_address_remote = "GJ";
    } else if (mac_address == "GJ") {
        m_address_remote = "24:D7:EB:41:7B:1C";
    } else {
        return -EINVAL;
    }
#else
    m_address_remote[0] = 0xFF;
    m_address_remote[1] = 0xFF;
    m_address_remote[2] = 0xFF;
    m_address_remote[3] = 0xFF;
    m_address_remote[4] = 0xFF;
    m_address_remote[5] = 0xFF;
#endif

    /* Add peer */
    memcpy(m_peer_info.peer_addr, m_address_remote, 6);
    m_peer_info.channel = 0;
    m_peer_info.encrypt = false;
    if (esp_now_add_peer(&m_peer_info) != ESP_OK) {
        Serial.println(" [e] Failed to add peer!\r\n");
        return -1;
    }

    /* Return success */
    return 0;
}

/**
 *
 */
int badge_remote::task(void) {

#if 0
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
#endif

    /* Return success */
    return 0;
}

/**
 *
 */
int badge_remote::message_available(void) {
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
int badge_remote::message_clear(void) {
    for (int i = 0; i < CONFIG_BADGE_MESSAGE_QUEUE_SIZE; i++) {
        m_queue_rx[i].pending = false;
    }
    return 0;
}

/**
 *
 */
int badge_remote::message_read(struct badge_message &msg) {
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
int badge_remote::message_write(struct badge_message msg) {

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
    if (esp_now_send(m_address_remote, data, 3) != ESP_OK) {
        Serial.printf(" [w] Failed to send message to remote badge!\r\n");
        return -EIO;
    }

    /* Return success */
    return 0;
}
