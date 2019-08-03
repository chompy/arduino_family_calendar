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

#include "State.h"
State* State::s_currentState = NULL;
uint8_t State::s_nextState = 255;

State::State()
{
}

State::~State()
{

}

void State::changeState(uint8_t stateId)
{
    State::s_nextState = stateId;
}

void State::changeState(State* state)
{
    // exit old state
    if (State::s_currentState) {
        State::s_currentState->exit();
        delete State::s_currentState;
        State::s_currentState = nullptr;
    }

    // reset next state flag
    State::s_nextState = 255;

    // no state provided
    if (!state) {
        return;
    }

    // enter new state
    State::s_currentState = state;
    State::s_currentState->enter();
}

bool State::hasNextState()
{
    return State::s_nextState != 255;
}

uint8_t State::getNextState()
{
    return State::s_nextState;
}

void State::loopCurrent()
{
    if (!State::s_currentState) {
        return;
    }
    State::s_currentState->loop();
}

void State::enter()
{

}

void State::loop()
{

}

void State::exit()
{

}