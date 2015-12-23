#ifndef __CAL_STATE_SPLASH__
#define __CAL_STATE_SPLASH__

#include "State.h"
#include "Utils.h"

#include "StateSetClock.h"

#define STATE_SPLASH_BG_COLOR 0x0000
#define STATE_SPLASH_TEXT_COLOR 0xFFFF
#define STATE_SPLASH_VERSION_Y 180
#define STATE_SPLASH_VERSION_LENGTH 12
#define STATE_SPLASH_TEXT_SIZE 2

#define STATE_SPLASH_DELAY 6000

class StateSplash : public State
{
public:

    static const uint8_t ID = 3;

    StateSplash(Utils* _utils) : State()
    {
        utils = _utils;
    }
    ~StateSplash();

    void enter();
    void loop();
    void exit();

private:
    Utils* utils;
};

#endif