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

#ifndef __CAL_STATE_SPLASH__
#define __CAL_STATE_SPLASH__

#include "State.h"
#include "Utils.h"

#include "StateSetClock.h"

#define STATE_SPLASH_BG_COLOR 0x0000
#define STATE_SPLASH_TEXT_COLOR 0xFFFF
#define STATE_SPLASH_VERSION_Y 180
#define STATE_SPLASH_VERSION_LENGTH 12
#define STATE_SPLASH_TEXT_SIZE 2

#define STATE_SPLASH_DELAY 6000

class StateSplash : public State
{
public:

    static const uint8_t ID = 3;

    StateSplash(Utils* _utils) : State()
    {
        utils = _utils;
    }
    ~StateSplash();

    void enter();
    void loop();
    void exit();

private:
    Utils* utils;
};

#endif