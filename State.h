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

#ifndef __CAL_STATE__
#define __CAL_STATE__

#include "SPI.h"

class State
{
public:
    State();
    virtual ~State();
    
    static void changeState(uint8_t stateId);
    static void changeState(State* state);

    static bool hasNextState();
    static uint8_t getNextState();
    
    static void loopCurrent();

    virtual void enter();
    virtual void loop();
    virtual void exit();

    static uint8_t s_nextState;
    static State* s_currentState;

};

#endif