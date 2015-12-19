#include "StateSetClock.h"

StateSetClock::~StateSetClock()
{

}

void StateSetClock::enter()
{
    shour = hour();
    sminute = minute();
    pm = isPM();
    smonth = month();
    sday = day();
    syear = year();

    // calculate position for time string
    timePosX = (tft->width() / 2) - (((Utils::FONT_SIZE_W * 16) * STATE_SETCLOCK_FONT_SIZE) / 2);    

    // draw one time elements
    tft->fillScreen(STATE_SETCLOCK_BG_COLOR);

    // up arrows
    uint8_t arrowCenter = ((((Utils::FONT_SIZE_W * 2) * STATE_SETCLOCK_FONT_SIZE) / 2) - (BITMAP_ARROW_UP_W / 2));
    for (uint8_t i = 0; i < 3; i++) {
        tft->drawBitmap(
            timePosX + (i * ((Utils::FONT_SIZE_W * 2) * 7)) + arrowCenter,
            32,
            BITMAP_ARROW_UP,
            BITMAP_ARROW_UP_W,
            BITMAP_ARROW_UP_H,
            STATE_SETCLOCK_ARROW_COLOR
        );
    }

    // down arrows
    for (uint8_t i = 0; i < 3; i++) {
        tft->drawBitmap(
            timePosX + (i * ((Utils::FONT_SIZE_W * 2) * 7)) + arrowCenter,
            96,
            BITMAP_ARROW_DOWN,
            BITMAP_ARROW_DOWN_W,
            BITMAP_ARROW_DOWN_H,
            STATE_SETCLOCK_ARROW_COLOR
        );
    }

    // draw elements that will be redrawn
    redraw();
}

void StateSetClock::loop()
{
    //Utils::isTouched(ts, tft, 16, 16, 32, 32);
}

void StateSetClock::exit()
{

}

void StateSetClock::redraw()
{

    char timeString[16];
    sprintf(
        timeString,
        "%s%d  :  %s%d  :  %s",
        (shour < 10) ? "0" : "",
        shour,
        (sminute < 10) ? "0" : "",
        sminute,
        pm ? "PM" : "AM"
    );
    Utils::drawString(
        tft,
        timePosX,
        64,
        timeString,
        STATE_SETCLOCK_TEXT_COLOR,
        STATE_SETCLOCK_BG_COLOR,
        STATE_SETCLOCK_FONT_SIZE
    );
}