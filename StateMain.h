#ifndef __CAL_STATE_MAIN__
#define __CAL_STATE_MAIN__

#include <Time.h>
#include "SPI.h"
#include "State.h"
#include "Utils.h"
#include "StateSetClock.h"

// Event parser settings
#define EVENT_DATA_LENGTH 26
#define EVENT_TITLE_LENGTH 24
#define EVENT_DATE_LENGTH 5
#define EVENT_TYPE_NONE 0
#define EVENT_TYPE_BDAY 1
#define EVENT_TYPE_HOLIDAY 2
#define EVENT_TYPE_ANIVERSARY 3
#define EVENT_TYPE_EVENT 4

// State settings
#define STATE_MAIN_BG_COLOR 0x0000
#define STATE_MAIN_EVENT_DATE_TEXT_COLOR 0x9e9e
#define STATE_MAIN_LIST_W 120
#define STATE_MAIN_TIME_Y 12
#define STATE_MAIN_TIME_LENGTH 8
#define STATE_MAIN_TIME_FONT_SIZE 2
#define STATE_MAIN_DATE_Y 38
#define STATE_MAIN_DATE_LENGTH 11
#define STATE_MAIN_DATE_FONT_SIZE 1
#define STATE_MAIN_EVENT_COUNT 5
#define STATE_MAIN_EVENT_X 12
#define STATE_MAIN_EVENT_Y 32
#define STATE_MAIN_EVENT_ICON_PAD 24
#define STATE_MAIN_EVENT_LINE_PAD 3
#define STATE_MAIN_EVENT_PAD 16
#define STATE_MAIN_EVENT_FONT_SIZE 1

#define STATE_MAIN_PHOTO_CHANGE_INTERVAL 60

class StateMain : public State
{
public:

    static const uint8_t ID = 1;

    StateMain(Utils* _utils) : State()
    {
        utils = _utils;
        photoSequence = nullptr;
    }
    ~StateMain();

    void enter();
    void loop();
    void exit();

private:
    Utils* utils;
    uint8_t lastMinute;
    uint8_t lastDay;

    uint8_t photoCount;
    time_t lastPhotoChange;
    uint8_t* photoSequence;
    uint8_t currentPhotoIndex;

};

#endif