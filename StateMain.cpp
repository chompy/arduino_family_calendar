#include "StateMain.h"

StateMain::~StateMain()
{

}

void StateMain::enter()
{
    // reset touch state
    utils->updateTouchState(-1, -1);

    // clear screen
    utils->tft->fillScreen(STATE_MAIN_BG_COLOR);

    // draw vertical seperator line
    utils->tft->drawFastVLine(
        utils->tft->width() - STATE_MAIN_LIST_W + 2,
        0,
        utils->tft->height(),
        DISPLAY_DATETIME_TEXT_COLOR
    );

    // reset vars
    lastMinute = 99;
    lastDay = 99;
    lastPhotoChange = 0;
    currentPhotoIndex = 0;
    setTimeHold = 0;

    // get number of photos on sd
    photoCount = 0;
    if (utils->fileOpen("photos.dat")) {
        photoCount = (uint8_t) utils->fileRead();
        utils->fileClose();
    }

    // build psuedo random photo sequence
    photoSequence = new uint8_t[photoCount];
    for (uint8_t i = 0; i < photoCount; i++) {
        while(true) {
            photoSequence[i] = random(0, photoCount);
            bool hasPhoto = false;
            for (uint8_t j = 0; j < i ; j++) {
                if (photoSequence[j] == photoSequence[i]) {
                    hasPhoto = true;
                    break;
                }
            }
            if (!hasPhoto) {
                break;
            }
        }
    }
}

void StateMain::loop()
{

    // only draw time if update is needed
    if (lastMinute != minute()) {
        lastMinute = minute();
        // generate time string
        char timeString[DISPLAY_TIME_LENGTH];
        sprintf(
            timeString,
            "%s%d:%s%d %s",
            ((hour() > 0 && hour() < 10) || (hour() > 12 && hour() < 22)) ? "0" : "",
            hour() == 0 ? 12 : (hour() > 12 ? hour() - 12 : hour()),
            (minute() < 10) ? "0" : "",
            minute(),
            isPM() ? "PM" : "AM"
        );
        // draw string
        utils->drawString(
            utils->tft->width() - STATE_MAIN_LIST_W + 2 + (STATE_MAIN_LIST_W / 2) - ((STATE_MAIN_TIME_LENGTH * (FONT_SIZE_W * STATE_MAIN_TIME_FONT_SIZE)) / 2),
            STATE_MAIN_TIME_Y,
            timeString,
            DISPLAY_DATETIME_TEXT_COLOR,
            DISPLAY_DATETIME_BG_COLOR,
            DISPLAY_TIME_FONT_SIZE
        );
    }

    // only draw date/events if update is needed
    if (lastDay != day()) {
        lastDay = day();
        // generate date string
        char dateString[DISPLAY_DATE_LENGTH] = "";
        sprintf(
            dateString,
            "%s %s%d %d",
            monthShortStr(month()),
            (day() < 10) ? "0" : "",
            day(),
            year()
        );
        // draw string
        utils->drawString(
            utils->tft->width() - STATE_MAIN_LIST_W + 2 + (STATE_MAIN_LIST_W / 2) - ((STATE_MAIN_DATE_LENGTH * (FONT_SIZE_W * STATE_MAIN_DATE_FONT_SIZE)) / 2),
            STATE_MAIN_DATE_Y,
            dateString,
            DISPLAY_DATETIME_TEXT_COLOR,
            DISPLAY_DATETIME_BG_COLOR,
            DISPLAY_DATE_FONT_SIZE
        ); 

        // attempt to open event file
        if (!utils->fileOpen("events.dat")) {
            return;
        }

        // get number of events
        uint8_t eventCount = (uint8_t) utils->fileRead();
        if (eventCount == 0) {
            utils->fileClose();
            return;
        }

        // iterate dates
        uint8_t counter = 0;
        utils->fileSeek((eventCount * EVENT_DATA_LENGTH) + 1);
        while(counter < STATE_MAIN_EVENT_COUNT) {
            
            uint8_t id = (uint8_t) utils->fileRead();
            uint8_t emonth = (uint8_t) utils->fileRead();
            uint8_t eday = (uint8_t) utils->fileRead();
            uint8_t yearBytes[2];
            yearBytes[0] = (uint8_t) utils->fileRead();
            yearBytes[1] = (uint8_t) utils->fileRead();
            uint16_t eyear = 0;
            eyear = (eyear << 8) + yearBytes[1];
            eyear = (eyear << 8) + yearBytes[0];

            if (eyear < year()) {
                continue;
            }
            if (eyear == year()) {
                if (emonth < month()) {
                    continue;
                }
                if (emonth == month()) {
                    if (eday < day()) {
                        continue;
                    }
                }
            }

            counter++;

            uint16_t currentPos = utils->filePosition();

            utils->fileSeek(1);
            while(true) {
                uint8_t _id = (uint8_t) utils->fileRead();
                if (_id == id) {

                    // get event type and draw related icon
                    uint8_t type = (uint8_t) utils->fileRead();

                    uint8_t typeImage = ICON_EVENT;
                    switch(type) {
                        case EVENT_TYPE_BDAY:
                        {
                            typeImage = ICON_BDAY;
                            break;
                        }
                        case EVENT_TYPE_ANIVERSARY:
                        {
                            typeImage = ICON_ANNIVERSARY;
                            break;
                        }
                    }

                    // store current position so we can open icons
                    uint16_t bitmapPosStash = utils->filePosition();

                    // open icons and draw needed icon
                    utils->fileOpen("icons.dat");
                    utils->fileSeek(1);
                    utils->drawImage(
                        utils->tft->width() - STATE_MAIN_LIST_W + 2 + STATE_MAIN_EVENT_X,
                        STATE_MAIN_EVENT_Y + ((((STATE_MAIN_EVENT_FONT_SIZE * FONT_SIZE_H) * 2) + STATE_MAIN_EVENT_PAD + STATE_MAIN_EVENT_LINE_PAD) * counter),
                        typeImage
                    );

                    // open event file back up
                    utils->fileOpen("events.dat");
                    utils->fileSeek(bitmapPosStash);
                    
                    // get title of event
                    uint8_t titleLen = EVENT_TITLE_LENGTH;
                    char title[EVENT_TITLE_LENGTH];
                    for (uint8_t i = 0; i < EVENT_TITLE_LENGTH; i++) {
                        title[i] = utils->fileRead();
                        if (title[i] == '\0' && titleLen >= EVENT_TITLE_LENGTH) {
                            titleLen = i + 1;
                        }
                    }
                    if (titleLen > DISPLAY_EVENT_LENGTH) {
                        title[DISPLAY_EVENT_LENGTH - 2] = '.';
                        title[DISPLAY_EVENT_LENGTH - 1] = '\0';
                    }

                    // draw title
                    utils->drawString(
                        utils->tft->width() - STATE_MAIN_LIST_W + 2 + STATE_MAIN_EVENT_X + STATE_MAIN_EVENT_ICON_PAD,
                        STATE_MAIN_EVENT_Y + ((((STATE_MAIN_EVENT_FONT_SIZE * FONT_SIZE_H) * 2) + STATE_MAIN_EVENT_PAD + STATE_MAIN_EVENT_LINE_PAD) * counter),
                        title,
                        DISPLAY_EVENT_TEXT_COLOR,
                        DISPLAY_EVENT_BG_COLOR,
                        DISPLAY_EVENT_FONT_SIZE
                    );

                    // generate date string
                    char dateString[DISPLAY_DATE_LENGTH] = "";
                    sprintf(
                        dateString,
                        "%s %s%d %d",
                        monthShortStr(emonth),
                        (eday < 10) ? "0" : "",
                        eday,
                        eyear
                    );
                    // draw date string
                    utils->drawString(
                        utils->tft->width() - STATE_MAIN_LIST_W + 2 + STATE_MAIN_EVENT_X + STATE_MAIN_EVENT_ICON_PAD,
                        STATE_MAIN_EVENT_Y + ((((STATE_MAIN_EVENT_FONT_SIZE * FONT_SIZE_H) * 2) + STATE_MAIN_EVENT_PAD + STATE_MAIN_EVENT_LINE_PAD) * counter) + (STATE_MAIN_EVENT_FONT_SIZE * FONT_SIZE_H) + STATE_MAIN_EVENT_LINE_PAD,
                        dateString,
                        STATE_MAIN_EVENT_DATE_TEXT_COLOR,
                        DISPLAY_DATETIME_BG_COLOR,
                        DISPLAY_EVENT_DATE_FONT_SIZE
                    );                 
                    break;
                }
                if (!utils->fileSeek( utils->filePosition() + EVENT_DATA_LENGTH - 1 )) {
                    break;
                }
            }
            if (!utils->fileSeek(currentPos)) {
                break;
            }
        }
        utils->fileClose();
    }
    
    // change photos
    if (photoCount > 0 && now() > lastPhotoChange + STATE_MAIN_PHOTO_CHANGE_INTERVAL) {
        lastPhotoChange = now();
        currentPhotoIndex++;

        if (currentPhotoIndex >= photoCount) {
            currentPhotoIndex = 0;
        }

        if (!utils->fileOpen("photos.dat")) {
            return;
        }

        // draw photo
        utils->fileSeek(1);
        utils->drawImage(0,0,photoSequence[currentPhotoIndex]);
        utils->fileClose();

    }

    // date/time area pressed (goto set clock)
    if (
        utils->isTouched(
            utils->tft->width() - STATE_MAIN_LIST_W,
            0,
            STATE_MAIN_LIST_W,
            STATE_MAIN_DATE_Y + (FONT_SIZE_H * STATE_MAIN_DATE_FONT_SIZE)
        )
    ) {

        if (setTimeHold == 0) {
            setTimeHold = millis();
        } else if (millis() > setTimeHold + STATE_MAIN_SET_TIME_HOLD) {
            State::changeState(
                StateSetClock::ID
            );
            setTimeHold = 0;
        }
    }

    if (setTimeHold > 0 && millis() > setTimeHold + STATE_MAIN_SET_TIME_HOLD) {
        setTimeHold = 0;
    }

}

void StateMain::exit()
{
    if (photoSequence) {
        delete photoSequence;
    }    
}