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