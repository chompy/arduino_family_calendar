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

#include "StateError.h"
char* StateError::message = nullptr;

StateError::~StateError()
{

}

void StateError::enter()
{
    utils->tft->fillScreen(STATE_ERROR_BG_COLOR);
    if (StateError::message) {
        uint16_t messageChars = strlen(StateError::message);
        utils->drawString(
            (utils->tft->width() / 2) - ((messageChars * FONT_SIZE_W) / 2),
            (utils->tft->height() / 2) - (FONT_SIZE_H / 2),
            StateError::message,
            STATE_ERROR_TEXT_COLOR,
            STATE_ERROR_BG_COLOR,
            1
        );
    }
}

void StateError::loop()
{
    return;
}

void StateError::exit()
{
    return;
}

void StateError::setMessage(char* _message)
{
    if (StateError::message) {
        delete StateError::message;
        StateError::message = nullptr;
    }
    StateError::message = new char[strlen(_message)];
    strcpy(StateError::message, _message);
}