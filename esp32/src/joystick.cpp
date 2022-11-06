/* Self header */
#include "joystick.h"

/* Arduino libraries */
#include <Arduino.h>
#include <WiFi.h>

/* */
class joystick joystick;

/**
 *
 */
int joystick::task(void) {

    /* State machine */
    switch (m_sm) {

        case STATE_0: {  // Detect if we have the correct mac address

            /* Proceed only if we are the badge with this hardware configuration */
            String mac_address = WiFi.macAddress();
            if (mac_address == "24:D7:EB:41:7B:1C") {
                m_sm = STATE_2;
            } else {
                m_sm = STATE_1;
            }
            break;
        }

        case STATE_1: {  // Do nothing
            break;
        }

        case STATE_2: {
            pinMode(CONFIG_BUTTON_LEFT_PIN, OUTPUT);
            pinMode(CONFIG_BUTTON_RIGHT_PIN, OUTPUT);
            digitalWrite(CONFIG_BUTTON_LEFT_PIN, HIGH);
            digitalWrite(CONFIG_BUTTON_RIGHT_PIN, HIGH);
            m_sm = STATE_3;
            break;
        }

        case STATE_3: {
            float value = -((analogRead(CONFIG_JOYSTICK_PIN) / 4095.0) - 0.5);
            if (value < -.1) {
                digitalWrite(CONFIG_BUTTON_LEFT_PIN, LOW);
                m_value = value;
                m_timestamp = millis();
                m_sm = STATE_4;
            } else if (value > .1) {
                digitalWrite(CONFIG_BUTTON_RIGHT_PIN, LOW);
                m_value = value;
                m_timestamp = millis();
                m_sm = STATE_4;
            }
            break;
        }

        case STATE_4: {

            /* Release button after 50ms */
            if (millis() - m_timestamp >= 50) {
                digitalWrite(CONFIG_BUTTON_LEFT_PIN, HIGH);
                digitalWrite(CONFIG_BUTTON_RIGHT_PIN, HIGH);
                m_timestamp = millis();
                m_sm = STATE_5;
            }
            break;
        }

        case STATE_5: {

            /* Abort early if we changed direction */
            float value = -((analogRead(CONFIG_JOYSTICK_PIN) / 4095.0) - 0.5);
            if ((value < 0 && m_value > 0) || (value > 0 && m_value < 0)) {
                m_sm = STATE_3;
                break;
            }

            /* Otherwise wait for an amount of time inversely proportional to the displacement of the joystick */
            long duration = map((1 - fabs(2 * value)) * 1000, 0, 1000, 100, 500);
            if (millis() - m_timestamp >= duration) {
                m_sm = STATE_3;
            }
            break;
        }

        default: {
            Serial.printf(" [e] Invalid state!");
            m_sm = STATE_0;
            return -1;
        }
    }

    /* Return success */
    return 0;
}
