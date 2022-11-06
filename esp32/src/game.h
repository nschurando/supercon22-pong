#ifndef GAME_H
#define GAME_H

/* Config */
#include "../cfg/config.h"

/* C/C++ libraries */
#include <errno.h>
#include <stdint.h>

/* */
class game {
   public:
    int task(void);

   protected:
    enum {
        STATE_0,
    } m_sm;
};

/* */
extern class game game;

#endif