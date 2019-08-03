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

#include "StateSplash.h"

StateSplash::~StateSplash()
{

}

void StateSplash::enter()
{

    if (utils->fileOpen("splash.dat")) {
        utils->fileSeek(1);
        utils->drawImage(
            0,
            0,
            0,
            320,
            240
        );
        utils->fileClose();
    }
    char versionString[STATE_SPLASH_VERSION_LENGTH];
    sprintf(
        versionString,
        "VERSION %d.%s%d",
        VERSION1,
        VERSION2 < 10 ? "0" : "",
        VERSION2        
    );
    utils->drawString(
        (utils->tft->width() / 2) - (((FONT_SIZE_W * STATE_SPLASH_TEXT_SIZE) * STATE_SPLASH_VERSION_LENGTH) / 2),
        STATE_SPLASH_VERSION_Y,
        versionString,
        STATE_SPLASH_TEXT_COLOR,
        STATE_SPLASH_BG_COLOR,
        STATE_SPLASH_TEXT_SIZE
    );

    delay(STATE_SPLASH_DELAY);
    utils->tft->fillScreen(STATE_SPLASH_BG_COLOR);
    State::changeState(
        StateSetClock::ID
    );
}

void StateSplash::loop()
{

}

void StateSplash::exit()
{

}