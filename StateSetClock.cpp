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

    // calculate position for time/date strings
    timePosX = (utils->tft->width() / 2) - (((FONT_SIZE_W * STATE_SETCLOCK_TIME_LENGTH) * STATE_SETCLOCK_FONT_SIZE) / 2);

    // clear screen
    utils->tft->fillScreen(STATE_SETCLOCK_BG_COLOR);

    // draw title text
    utils->drawString(
        STATE_SETCLOCK_TITLE_X,
        STATE_SETCLOCK_TITLE_Y,
        "Set Time & Date",
        STATE_SETCLOCK_TEXT_COLOR,
        STATE_SETCLOCK_BG_COLOR,
        STATE_SETCLOCK_FONT_SIZE
    );

    utils->tft->drawFastHLine(
        0,
        STATE_SETCLOCK_TITLE_Y + (FONT_SIZE_H * STATE_SETCLOCK_FONT_SIZE) + 4,
        utils->tft->width(),
        STATE_SETCLOCK_TEXT_COLOR
    );

    // done button
    /*utils->tft->fillRect(
        0,
        utils->tft->height() - STATE_SETCLOCK_DONE_H,
        utils->tft->width(),
        STATE_SETCLOCK_DONE_H,
        STATE_SETCLOCK_DONE_BG_COLOR
    );
    utils->drawString(
        (utils->tft->width() / 2) - (((4 * FONT_SIZE_W) * STATE_SETCLOCK_FONT_SIZE) / 2),
        (utils->tft->height() - STATE_SETCLOCK_DONE_H) + 1 + (STATE_SETCLOCK_DONE_H / 2) - ((FONT_SIZE_H * STATE_SETCLOCK_FONT_SIZE) / 2),
        "DONE",
        STATE_SETCLOCK_DONE_TEXT_COLOR,
        STATE_SETCLOCK_DONE_BG_COLOR,
        STATE_SETCLOCK_FONT_SIZE
    );
    utils->tft->drawFastHLine(
        0,
        utils->tft->height() - STATE_SETCLOCK_DONE_H,
        utils->tft->width(),
        STATE_SETCLOCK_DONE_SEP_COLOR
    );*/

    // up arrows
    /*uint8_t arrowCenter = ((((FONT_SIZE_W * 2) * STATE_SETCLOCK_FONT_SIZE) / 2) - (STATE_SETCLOCK_ARROW_W / 2));
    for (uint8_t i = 0; i < 3; i++) {
        utils->drawBitmap(
            "icons/arr_up.bmp",
            timePosX + (i * ((FONT_SIZE_W * STATE_SETCLOCK_FONT_SIZE) * 7)) + arrowCenter,
            STATE_SETCLOCK_TIME_Y - STATE_SETCLOCK_ARROW_H - STATE_SETCLOCK_ARROW_SPACING
        );
        utils->drawBitmap(
            "icons/arr_up.bmp",
            timePosX + (i * ((FONT_SIZE_W * STATE_SETCLOCK_FONT_SIZE) * 7)) + arrowCenter,
            STATE_SETCLOCK_DATE_Y - STATE_SETCLOCK_ARROW_H - STATE_SETCLOCK_ARROW_SPACING
        );
    }

    // down arrows
    for (uint8_t i = 0; i < 3; i++) {
        utils->drawBitmap(
            "icons/arr_dwn.bmp",
            timePosX + (i * ((FONT_SIZE_W * STATE_SETCLOCK_FONT_SIZE) * 7)) + arrowCenter,
            STATE_SETCLOCK_TIME_Y + ((FONT_SIZE_H - 1) * STATE_SETCLOCK_FONT_SIZE) + STATE_SETCLOCK_ARROW_SPACING
        );
        utils->drawBitmap(
            "icons/arr_dwn.bmp",
            timePosX + (i * ((FONT_SIZE_W * STATE_SETCLOCK_FONT_SIZE) * 7)) + arrowCenter,
            STATE_SETCLOCK_DATE_Y + ((FONT_SIZE_H - 1) * STATE_SETCLOCK_FONT_SIZE) + STATE_SETCLOCK_ARROW_SPACING
        );
    }*/

    // draw elements that will be redrawn
    redraw();

    delay(4000);
    State::changeState(
        StateMain::ID
    );
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

    char timeString[STATE_SETCLOCK_TIME_LENGTH];
    sprintf(
        timeString,
        "%s%d  :  %s%d  :  %s",
        (shour < 10) ? "0" : "",
        shour,
        (sminute < 10) ? "0" : "",
        sminute,
        pm ? "PM" : "AM"
    );
    utils->drawString(
        timePosX,
        STATE_SETCLOCK_TIME_Y,
        timeString,
        STATE_SETCLOCK_TEXT_COLOR,
        STATE_SETCLOCK_BG_COLOR,
        STATE_SETCLOCK_FONT_SIZE
    );

    char dateString[STATE_SETCLOCK_DATE_LENGTH];
    sprintf(
        dateString,
        "%s%d  /  %s%d  /  %d",
        (smonth < 10) ? "0" : "",
        smonth,
        (sday < 10) ? "0" : "",
        sday,
        syear
    );
    utils->drawString(
        timePosX,
        STATE_SETCLOCK_DATE_Y,
        dateString,
        STATE_SETCLOCK_TEXT_COLOR,
        STATE_SETCLOCK_BG_COLOR,
        STATE_SETCLOCK_FONT_SIZE
    );

}