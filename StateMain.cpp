#include "StateMain.h"

StateMain::~StateMain()
{

}

void StateMain::enter()
{

    // clear screen
    tft->fillScreen(STATE_MAIN_BG_COLOR);

    // reset vars
    lastMinute = 99;
    lastDay = 99;

    // align right
    if (eventAlign) {
        eventXPos = tft->width() - STATE_MAIN_EVENT_LIST_W;
        tft->drawFastVLine(
            eventXPos + 2,
            0,
            tft->height(),
            STATE_MAIN_TEXT_COLOR
        );

    // align left
    } else {
        eventXPos = 0;
        tft->drawFastVLine(
            eventXPos + STATE_MAIN_EVENT_LIST_W - 2,
            0,
            tft->height(),
            STATE_MAIN_TEXT_COLOR
        );
    }    

    timeXPos = eventXPos + 2 + ((STATE_MAIN_EVENT_LIST_W / 2) - (((Utils::FONT_SIZE_W * STATE_MAIN_EVENT_TIME_LENGTH) * STATE_MAIN_EVENT_TIME_FONT_SIZE) / 2));
    dateXPos = eventXPos + 2 + ((STATE_MAIN_EVENT_LIST_W / 2) - (((Utils::FONT_SIZE_W * STATE_MAIN_EVENT_DATE_LENGTH) * STATE_MAIN_EVENT_DATE_FONT_SIZE) / 2));
}

void StateMain::loop()
{
    drawDateTime();
    return;
}

void StateMain::exit()
{
    return;
}

void StateMain::drawDateTime()
{
    // only draw time if update is needed
    if (lastMinute != minute()) {
        lastMinute = minute();
        // get hour in 12hour format
        uint8_t tHour = hour();
        if (isPM()) {
            tHour = tHour -12;
        } else if (tHour == 0 ) {
            tHour = 12;
        }
        // generate time string
        char timeString[STATE_MAIN_EVENT_TIME_LENGTH];
        sprintf(
            timeString,
            "%s%d:%s%d %s",
            (tHour < 10) ? "0" : "",
            tHour,
            (minute() < 10) ? "0" : "",
            minute(),
            isPM() ? "PM" : "AM"
        );
        // draw string
        Utils::drawString(
            tft,
            timeXPos,
            STATE_MAIN_EVENT_TIME_Y,
            timeString,
            STATE_MAIN_TEXT_COLOR,
            STATE_MAIN_BG_COLOR,
            STATE_MAIN_EVENT_TIME_FONT_SIZE
        );
    }

    // only draw date if update is needed
    if (lastDay != day()) {
        lastDay = day();
        // generate date string
        char dateString[STATE_MAIN_EVENT_DATE_LENGTH];
        sprintf(
            dateString,
            "%s%d/%s%d/%d",
            (month() < 10) ? "0" : "",
            month(),
            (day() < 10) ? "0" : "",
            day(),
            year()
        );
        // draw string
        Utils::drawString(
            tft,
            dateXPos,
            STATE_MAIN_EVENT_DATE_Y,
            dateString,
            STATE_MAIN_TEXT_COLOR,
            STATE_MAIN_BG_COLOR,
            STATE_MAIN_EVENT_DATE_FONT_SIZE
        );
    }
}