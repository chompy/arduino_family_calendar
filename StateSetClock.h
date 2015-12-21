#ifndef __CAL_STATE_SETCLOCK__
#define __CAL_STATE_SETCLOCK__

#include <Time.h>
#include "SPI.h"
#include "State.h"
#include "Utils.h"

#include "StateMain.h"

#define STATE_SETCLOCK_BG_COLOR 0x0000
#define STATE_SETCLOCK_TEXT_COLOR 0xFFFF

#define STATE_SETCLOCK_DONE_BG_COLOR 0xF800
#define STATE_SETCLOCK_DONE_TEXT_COLOR 0xFFFF
#define STATE_SETCLOCK_DONE_SEP_COLOR 0xFFFF
#define STATE_SETCLOCK_DONE_H 32

#define STATE_SETCLOCK_TITLE_X 8
#define STATE_SETCLOCK_TITLE_Y 8
#define STATE_SETCLOCK_ARROW_W 16
#define STATE_SETCLOCK_ARROW_H 16
#define STATE_SETCLOCK_ARROW_SPACING 8
#define STATE_SETCLOCK_FONT_SIZE 2
#define STATE_SETCLOCK_TIME_Y 68
#define STATE_SETCLOCK_TIME_LENGTH 16
#define STATE_SETCLOCK_DATE_Y 148
#define STATE_SETCLOCK_DATE_LENGTH 18

class StateSetClock : public State
{
public:

    static const uint8_t ID = 2;

    StateSetClock(Utils* _utils) : State()
    {
        utils = _utils;
    }
    ~StateSetClock();

    void enter();
    void loop();
    void exit();

private:
    Utils* utils;
    
    uint16_t timePosX;

    uint8_t shour;
    uint8_t sminute;
    bool pm;

    uint8_t smonth;
    uint8_t sday;
    uint16_t syear;

    void redraw();

};


#endif