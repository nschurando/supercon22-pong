/* Project code */
#include "badge_local.h"
#include "game.h"
#include "joystick.h"

/* Arduino libraries */
#include <Arduino.h>
#include <WiFi.h>

/* Esp32 libraries */
#include <esp_now.h>

/* Config */
#include "../cfg/config.h"

/**
 *
 */
void setup() {

    /* Setup serial to computer */
    Serial.begin(115200);
    Serial.println(" [i] Hello world");

    /* Setup wi-fi */
    Serial.print(" [i] MAC Address: ");
    Serial.println(WiFi.macAddress());
    WiFi.mode(WIFI_STA);
}

/**
 *
 */
void loop() {
    int res;

    /* Handle incoming messages from badge */
    res = badge_local.task();
    if (res < 0) {
        Serial.printf(" [e] Error while processing local badge messages!\r\n");
    }

    /* Handle game */
    res = game.task();
    if (res < 0) {
        Serial.printf(" [e] Error while managing game!\r\n");
    }

    /* Handle joystick */
    res = joystick.task();
    if (res < 0) {
        Serial.printf(" [e] Error while managing joystick!\r\n");
    }
}
