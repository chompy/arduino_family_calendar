#ifndef __CAL_STATE_SETCLOCK__
#define __CAL_STATE_SETCLOCK__

#include <Time.h>
#include <string.h>

#include "SPI.h"
#include <Adafruit_TFTLCD.h>
#include <TouchScreen.h>

#include "State.h"
#include "Utils.h"

#define STATE_SETCLOCK_BG_COLOR 0x0000
#define STATE_SETCLOCK_TEXT_COLOR 0xFFFF
#define STATE_SETCLOCK_ARROW_COLOR 0xF800
#define STATE_SETCLOCK_FONT_SIZE 2

class StateSetClock : public State
{
public:
    StateSetClock(Adafruit_TFTLCD* _tft, TouchScreen* _ts) : State()
    {
        tft = _tft;
        ts = _ts;
    }
    ~StateSetClock();

    void enter();
    void loop();
    void exit();

private:
    Adafruit_TFTLCD* tft;
    TouchScreen* ts;
    
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