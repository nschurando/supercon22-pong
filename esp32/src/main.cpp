/* Arduino libraries */
#include <Arduino.h>

/* Config */
#include "../cfg/config.h"

/**
 *
 */
void setup() {
    // DEBUG monitoring

    /* Setup serial to computer */
    Serial.begin(115200);
    Serial.println(" [i] Hello world");

    /* Setup serial to badge */
    Serial2.begin(9600, SERIAL_8N1, CONFIG_BADGELINK_RX_PIN, CONFIG_BADGELINK_TX_PIN);
}

/**
 *
 */
void loop() {

    /* For now, echo what badge emits */
    while (Serial2.available() > 0) {
        Serial.printf(" [d] badge says: 0x%02X\r\n", Serial2.read());
    }
}
