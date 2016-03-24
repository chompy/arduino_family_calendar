#ifndef __CAL_STATE_UPLOAD__
#define __CAL_STATE_UPLOAD__

#include <SPI.h>
#include "State.h"
#include "Utils.h"

#include "StateSetClock.h"

#define STATE_UPLOAD_BG_COLOR 0x0000
#define STATE_UPLOAD_TEXT_COLOR 0xFFFF
#define STATE_UPLOAD_TEXT_SIZE 2

#define SP_STATE_NONE 0
#define SP_STATE_WAIT 1
#define SP_STATE_PHOTO_COUNT 2
#define SP_STATE_PHOTO_REQUEST 3
#define SP_STATE_PHOTO_DOWNLOAD 4

class StateUpload : public State
{
public:

    static const uint8_t ID = 4;

    StateUpload(Utils* _utils) : State()
    {
        utils = _utils;
    }
    ~StateUpload();

    void enter();
    void loop();
    void exit();

private:
    Utils* utils;
    uint8_t state;
    uint8_t photoRequest;
};

#endif