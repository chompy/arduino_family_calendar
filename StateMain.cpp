#include "StateMain.h"

StateMain::~StateMain()
{

}

void StateMain::enter()
{
    // clear screen
    utils->tft->fillScreen(STATE_MAIN_BG_COLOR);

    // draw vertical seperator line
    utils->tft->drawFastVLine(
        utils->tft->width() - STATE_MAIN_LIST_W + 2,
        0,
        utils->tft->height(),
        STATE_MAIN_TEXT_COLOR
    );

    // reset vars
    lastMinute = 99;
    lastDay = 99;
    lastPhotoChange = 0;
    currentPhotoIndex = 0;

    // get number of photos on sd
    photoCount = 0;
    if (SD.exists("photos")) {
        File photoDir = SD.open("photos");
        while(true) {
            File photo = photoDir.openNextFile();
            if (!photo) {
                break;
            }
            photo.close();
            photoCount++;
        }
        photoDir.close();
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
                Serial.println(photoSequence[i]);
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
            (hour() > 0 && hour() < 10 ) ? "0" : "",
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

        // load event data from sd card
        if (!SD.exists("events.dat")) {
            return;
        }
        File eventData = SD.open("events.dat");

        // read first byte
        int8_t byte = eventData.read();
        if (byte == -1) {
            return;
        }

        // get number of events
        uint8_t eventCount = (uint8_t) byte;
        if (eventCount == 0) {
            return;
        }

        // iterate dates
        uint8_t counter = 0;
        eventData.seek((eventCount * EVENT_DATA_LENGTH) + 1);
        while(counter < STATE_MAIN_EVENT_COUNT) {
            
            if (eventData.position() + EVENT_DATE_LENGTH > eventData.size()) {
                break;
            }

            uint8_t id = (uint8_t) eventData.read();
            uint8_t emonth = (uint8_t) eventData.read();
            uint8_t eday = (uint8_t) eventData.read();
            uint8_t yearBytes[2];
            yearBytes[0] = (uint8_t) eventData.read();
            yearBytes[1] = (uint8_t) eventData.read();
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

            uint16_t currentPos = eventData.position();

            eventData.seek(1);
            while(true) {
                uint8_t _id = (uint8_t) eventData.read();
                if (_id == id) {

                    // get event type and draw related icon
                    uint8_t type = (uint8_t) eventData.read();
                    switch (type)
                    {
                        case EVENT_TYPE_BDAY:
                        {
                            utils->drawBitmap(
                                "icons/bday.bmp",
                                utils->tft->width() - STATE_MAIN_LIST_W + 2 + STATE_MAIN_EVENT_X,
                                STATE_MAIN_EVENT_Y + ((((STATE_MAIN_EVENT_FONT_SIZE * FONT_SIZE_H) * 2) + STATE_MAIN_EVENT_PAD + STATE_MAIN_EVENT_LINE_PAD) * counter)
                            );
                            break;
                        }
                        case EVENT_TYPE_ANIVERSARY:
                        {
                            utils->drawBitmap(
                                "icons/anni.bmp",
                                utils->tft->width() - STATE_MAIN_LIST_W + 2 + STATE_MAIN_EVENT_X,
                                STATE_MAIN_EVENT_Y + ((((STATE_MAIN_EVENT_FONT_SIZE * FONT_SIZE_H) * 2) + STATE_MAIN_EVENT_PAD + STATE_MAIN_EVENT_LINE_PAD) * counter)
                            );
                            break;
                        }
                        default:
                        {
                            utils->drawBitmap(
                                "icons/default.bmp",
                                utils->tft->width() - STATE_MAIN_LIST_W + 2 + STATE_MAIN_EVENT_X,
                                STATE_MAIN_EVENT_Y + ((((STATE_MAIN_EVENT_FONT_SIZE * FONT_SIZE_H) * 2) + STATE_MAIN_EVENT_PAD + STATE_MAIN_EVENT_LINE_PAD) * counter)
                            );                            
                            break;
                        }
                    }

                    // get title of event
                    uint8_t titleLen = EVENT_TITLE_LENGTH;
                    char title[EVENT_TITLE_LENGTH];
                    for (uint8_t i = 0; i < EVENT_TITLE_LENGTH; i++) {
                        title[i] = eventData.read();
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
                eventData.seek( eventData.position() + EVENT_DATA_LENGTH - 1 );
            }
            eventData.seek(currentPos);
        }
        eventData.close();
    }
    
    // change photos
    if (photoCount > 0 && now() > lastPhotoChange + STATE_MAIN_PHOTO_CHANGE_INTERVAL) {
        lastPhotoChange = now();
        
        currentPhotoIndex++;
        if (currentPhotoIndex >= photoCount) {
            currentPhotoIndex = 0;
        }

        if (SD.exists("photos")) {
            File photoDir = SD.open("photos");
            for (uint8_t i = 0; i <= photoSequence[currentPhotoIndex]; i++) {
                File photo = photoDir.openNextFile();
                if (!photo) {
                    break;
                }
                if (i == photoSequence[currentPhotoIndex]) {
                    char photoFilename[32];
                    sprintf(
                        photoFilename,
                        "photos/%s",
                        photo.name()
                    );
                    utils->drawBitmap(
                        photoFilename,
                        0,
                        0
                    );
                }
                photo.close();
            }
            photoDir.close();
        }
    }

}

void StateMain::exit()
{
    if (photoSequence) {
        delete photoSequence;
    }    
}