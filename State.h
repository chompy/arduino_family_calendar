#ifndef __CAL_STATE__
#define __CAL_STATE__

class State
{
public:
    State();
    virtual ~State();
    static void changeState(State* _state);
    static void loopCurrent();

    virtual void enter();
    virtual void loop();
    virtual void exit();

    static State* s_currentState;
};

#endif