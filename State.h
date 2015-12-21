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