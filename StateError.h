#ifndef __CAL_STATE_ERROR__
#define __CAL_STATE_ERROR__

#include <string.h>
#include "SPI.h"
#include "State.h"
#include "Utils.h"

#define STATE_ERROR_BG_COLOR 0x0000
#define STATE_ERROR_BITMAP_COLOR 0xFFE0
#define STATE_ERROR_TEXT_COLOR 0xFFFF

class StateError : public State
{
public:

    static const uint8_t ID = 0;

    StateError(Utils* _utils) : State()
    {
        utils = _utils;
    }
    ~StateError();

    void enter();
    void loop();
    void exit();

    static void setMessage(char* _message);

private:
    Utils* utils;
    static char* message;
};


#endif