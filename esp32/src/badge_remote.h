#ifndef BADGE_REMOTE_H
#define BADGE_REMOTE_H

/* Config */
#include "../cfg/config.h"

/* Project code */
#include "badge_message.h"

/* Arduino libraries */
#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

/* C/C++ libraries */
#include <errno.h>
#include <stdint.h>

/* Class for exchanging messages with remote badge */
class badge_remote {
   public:
    int setup(void);
    int task(void);
    int message_available(void);
    int message_clear(void);
    int message_read(struct badge_message &msg);
    int message_write(struct badge_message msg);
    friend void bytes_received(const uint8_t *mac_addr, const uint8_t *data, int data_len);
    friend void data_sent(const uint8_t *mac_addr, esp_now_send_status_t status);

   protected:
    struct {
        bool pending;
        struct badge_message msg;
    } m_queue_rx[CONFIG_BADGE_MESSAGE_QUEUE_SIZE] = {0};
    int m_queue_rx_index = 0;
    uint8_t m_address_remote[6] = {0};
    esp_now_peer_info_t m_peer_info;
};

/* Global instance */
extern class badge_remote badge_remote;

#endif
