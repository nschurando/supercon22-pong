#ifndef JOYSTICK_H
#define JOYSTICK_H

/* Config */
#include "../cfg/config.h"

/* C/C++ libraries */
#include <errno.h>
#include <stdint.h>

/* */
class joystick {
   public:
    int task(void);

   protected:
    float m_value = 0;
    uint32_t m_timestamp = 0;
    enum {
        STATE_0,
        STATE_1,
        STATE_2,
        STATE_3,
        STATE_4,
        STATE_5,
    } m_sm = STATE_0;
};

/* */
extern class joystick joystick;

#endif
