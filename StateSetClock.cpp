/*
This file is part of "arduino_family_calendar".

"arduino_family_calendar" is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"arduino_family_calendar" is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "arduino_family_calendar".  If not, see <https://www.gnu.org/licenses/>.
*/

#include "StateSetClock.h"

StateSetClock::~StateSetClock()
{

}

void StateSetClock::enter()
{
    shour = hour();
    sminute = minute();
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
    utils->tft->fillRect(
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
    );

    // open icons file
    if (utils->fileOpen("icons.dat")) {

        // up arrows
        uint8_t arrowCenter = ((((FONT_SIZE_W * 2) * STATE_SETCLOCK_FONT_SIZE) / 2) - (STATE_SETCLOCK_ARROW_W / 2));
        for (uint8_t i = 0; i < 3; i++) {
            utils->fileSeek(1);
            utils->drawImage(
                timePosX + (i * ((FONT_SIZE_W * STATE_SETCLOCK_FONT_SIZE) * 7)) + arrowCenter,
                STATE_SETCLOCK_TIME_Y - STATE_SETCLOCK_ARROW_H - STATE_SETCLOCK_ARROW_SPACING - 1,
                ICON_ARROW_UP,
                16,
                16
            );
            utils->fileSeek(1);
            utils->drawImage(
                timePosX + (i * ((FONT_SIZE_W * STATE_SETCLOCK_FONT_SIZE) * 7)) + arrowCenter,
                STATE_SETCLOCK_DATE_Y - STATE_SETCLOCK_ARROW_H - STATE_SETCLOCK_ARROW_SPACING - 1,
                ICON_ARROW_UP,
                16,
                16
            );
        }

        // down arrows
        for (uint8_t i = 0; i < 3; i++) {
            utils->fileSeek(1);
            utils->drawImage(
                timePosX + (i * ((FONT_SIZE_W * STATE_SETCLOCK_FONT_SIZE) * 7)) + arrowCenter,
                STATE_SETCLOCK_TIME_Y + ((FONT_SIZE_H - 1) * STATE_SETCLOCK_FONT_SIZE) + STATE_SETCLOCK_ARROW_SPACING,
                ICON_ARROW_DOWN,
                16,
                16
            );
            utils->fileSeek(1);
            utils->drawImage(
                timePosX + (i * ((FONT_SIZE_W * STATE_SETCLOCK_FONT_SIZE) * 7)) + arrowCenter,
                STATE_SETCLOCK_DATE_Y + ((FONT_SIZE_H - 1) * STATE_SETCLOCK_FONT_SIZE) + STATE_SETCLOCK_ARROW_SPACING,
                ICON_ARROW_DOWN,
                16,
                16
            );           
        }
    }

    // draw elements that will be redrawn
    redraw();

}

void StateSetClock::loop()
{

    uint8_t arrowCenter = ((((FONT_SIZE_W * 2) * STATE_SETCLOCK_FONT_SIZE) / 2) - (STATE_SETCLOCK_ARROW_W / 2));
    for (uint8_t i = 0; i < 3; i++) {

        // time upper
        if (
            utils->isTouched(
                timePosX + (i * ((FONT_SIZE_W * STATE_SETCLOCK_FONT_SIZE) * 7)) + arrowCenter - (STATE_SETCLOCK_ARROW_BUTTON_W / 2),
                STATE_SETCLOCK_TIME_Y - STATE_SETCLOCK_ARROW_H - STATE_SETCLOCK_ARROW_SPACING - (STATE_SETCLOCK_ARROW_BUTTON_H / 2),
                STATE_SETCLOCK_ARROW_BUTTON_W,
                STATE_SETCLOCK_ARROW_BUTTON_H
            )
        ) {

            switch(i) {
                case 0:
                {
                    shour++;
                    if (shour > 23) {
                        shour = 0;
                    }
                    break;
                }
                case 1:
                {
                    sminute++;
                    if (sminute > 59) {
                        sminute = 0;
                    }
                    break;
                }
                case 2:
                {
                    if (shour > 11) {
                        shour -= 12;
                    } else {
                        shour += 12;
                    }
                    break;
                }
            }
            redraw();
        }

        // time lower
        if (
            utils->isTouched(
                timePosX + (i * ((FONT_SIZE_W * STATE_SETCLOCK_FONT_SIZE) * 7)) + arrowCenter - (STATE_SETCLOCK_ARROW_BUTTON_W / 2),
                STATE_SETCLOCK_TIME_Y + ((FONT_SIZE_H - 1) * STATE_SETCLOCK_FONT_SIZE) + STATE_SETCLOCK_ARROW_SPACING - (STATE_SETCLOCK_ARROW_BUTTON_H / 2),
                STATE_SETCLOCK_ARROW_BUTTON_W,
                STATE_SETCLOCK_ARROW_BUTTON_H
            )
        ) {

            switch(i) {
                case 0:
                {
                    shour--;
                    if (shour < 0) {
                        shour = 23;
                    }
                    break;
                }
                case 1:
                {
                    sminute--;
                    if (sminute < 0) {
                        sminute = 59;
                    }
                    break;
                }
                case 2:
                {
                    if (shour > 11) {
                        shour -= 12;
                    } else {
                        shour += 12;
                    }
                    break;
                }
            }
            redraw();

        }

        // date upper
        if (
            utils->isTouched(
                timePosX + (i * ((FONT_SIZE_W * STATE_SETCLOCK_FONT_SIZE) * 7)) + arrowCenter - (STATE_SETCLOCK_ARROW_BUTTON_W / 2),
                STATE_SETCLOCK_DATE_Y - STATE_SETCLOCK_ARROW_H - STATE_SETCLOCK_ARROW_SPACING - (STATE_SETCLOCK_ARROW_BUTTON_W / 2),
                STATE_SETCLOCK_ARROW_BUTTON_W,
                STATE_SETCLOCK_ARROW_BUTTON_H
            )
        ) {

            switch(i) {
                case 0:
                {
                    smonth++;
                    if (smonth > 12) {
                        smonth = 1;
                    }
                    if (sday > utils->daysInMonth(smonth, syear)) {
                        sday = utils->daysInMonth(smonth, syear);
                    }
                    break;
                }
                case 1:
                {
                    sday++;
                    if (sday > utils->daysInMonth(smonth, syear)) {
                        sday = 1;
                    }
                    break;
                }
                case 2:
                {
                    syear++;
                    if (syear > 9999) {
                        syear = 1970;
                    }
                    if (smonth == 2 && sday > utils->daysInMonth(smonth, syear)) {
                        sday = utils->daysInMonth(smonth, syear);
                    }
                    break;
                }
            }
            redraw();
        }

        // date lower
        if (
            utils->isTouched(
                timePosX + (i * ((FONT_SIZE_W * STATE_SETCLOCK_FONT_SIZE) * 7)) + arrowCenter - (STATE_SETCLOCK_ARROW_BUTTON_W / 2),
                STATE_SETCLOCK_DATE_Y + ((FONT_SIZE_H - 1) * STATE_SETCLOCK_FONT_SIZE) + STATE_SETCLOCK_ARROW_SPACING - (STATE_SETCLOCK_ARROW_BUTTON_H / 2),
                STATE_SETCLOCK_ARROW_BUTTON_W,
                STATE_SETCLOCK_ARROW_BUTTON_H
            )
        ) {

            switch(i) {
                case 0:
                {
                    smonth--;
                    if (smonth < 1) {
                        smonth = 12;
                    }
                    if (sday > utils->daysInMonth(smonth, syear)) {
                        sday = utils->daysInMonth(smonth, syear);
                    }
                    break;
                }
                case 1:
                {
                    sday--;
                    if (sday < 1) {
                        sday = utils->daysInMonth(smonth, syear);
                    }
                    break;
                }
                case 2:
                {
                    syear--;
                    if (syear < 1970) {
                        syear = 9999;
                    }
                    if (smonth == 2 && sday > utils->daysInMonth(smonth, syear)) {
                        sday = utils->daysInMonth(smonth, syear);
                    }
                    break;
                }
            }
            redraw();

        }

    }

    // done button
    if (
        utils->isTouched(
            0,
            utils->tft->height() - STATE_SETCLOCK_DONE_H,
            utils->tft->width(),
            STATE_SETCLOCK_DONE_H
        )
    ) {
        // set time
        setTime(
            shour,
            sminute,
            0,
            sday,
            smonth,
            syear
        );
        randomSeed(now());

        // goto main state
        State::changeState(
            StateMain::ID
        );
    }


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
        ((shour > 0 && shour < 10) || (shour > 12 && shour < 22)) ? "0" : "",
        shour == 0 ? 12 : (shour > 12 ? shour - 12 : shour),
        (sminute < 10) ? "0" : "",
        sminute,
        shour > 11 ? "PM" : "AM"
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