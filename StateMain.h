#ifndef __CAL_STATE_MAIN__
#define __CAL_STATE_MAIN__

#include <Time.h>
#include <string.h>

#include "SPI.h"
#include <Adafruit_TFTLCD.h>
#include "State.h"
#include "Utils.h"

#define STATE_MAIN_BG_COLOR 0x0000
#define STATE_MAIN_BITMAP_COLOR 0xFFE0
#define STATE_MAIN_TEXT_COLOR 0xFFFF

#define STATE_MAIN_EVENT_LIST_W 120
#define STATE_MAIN_EVENT_ALIGN_DEFAULT 1
#define STATE_MAIN_EVENT_TIME_Y 8
#define STATE_MAIN_EVENT_TIME_LENGTH 8
#define STATE_MAIN_EVENT_TIME_FONT_SIZE 2
#define STATE_MAIN_EVENT_DATE_Y 32
#define STATE_MAIN_EVENT_DATE_LENGTH 10
#define STATE_MAIN_EVENT_DATE_FONT_SIZE 1

class StateMain : public State
{
public:
    StateMain(Adafruit_TFTLCD* _tft) : State()
    {
        tft = _tft;
        eventAlign = STATE_MAIN_EVENT_ALIGN_DEFAULT;
    }
    ~StateMain();

    void enter();
    void loop();
    void exit();

private:
    Adafruit_TFTLCD* tft;
    bool eventAlign;
    uint16_t eventXPos;
    uint16_t timeXPos;
    uint16_t dateXPos;
    uint8_t lastMinute;
    uint8_t lastDay;

    void drawDateTime();

};

#endif