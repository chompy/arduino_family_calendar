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

#ifndef __CAL_STATE_UPLOAD__
#define __CAL_STATE_UPLOAD__

#include <SPI.h>
#include "State.h"
#include "Utils.h"

#include "StateSetClock.h"

#define STATE_UPLOAD_BG_COLOR 0x0000
#define STATE_UPLOAD_TEXT_COLOR 0xFFFF
#define STATE_UPLOAD_TEXT_SIZE 2

#define SEND_BUF_SIZE 128

#define SP_STATE_NONE 0
#define SP_STATE_WAIT 1
#define SP_STATE_PHOTO_SEND 2
#define SP_STATE_PHOTO_RECV 3

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

    void displayWait();

    Utils* utils;
    uint8_t state;
    uint32_t photoDownloadSize;
};

#endif