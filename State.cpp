#include "State.h"
State* State::s_currentState = nullptr;

State::State()
{

}

State::~State()
{

}

void State::changeState(State* _state)
{
	if (State::s_currentState) {
		State::s_currentState->exit();
		delete State::s_currentState;
	}
	State::s_currentState = _state;
	State::s_currentState->enter();
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