#include "StateError.h"

StateError::~StateError()
{

}

void StateError::enter()
{
    tft->fillScreen(STATE_ERROR_BG_COLOR);
    uint16_t messageChars = strlen(message);
    Utils::drawString(
        tft,
        (tft->width() / 2) - ((messageChars * Utils::FONT_SIZE_W) / 2),
        tft->height() - 48,
        message,
        STATE_ERROR_TEXT_COLOR,
        STATE_ERROR_BG_COLOR,
        1
    );
    tft->drawBitmap(
        (tft->width() / 2) - (STATE_ERROR_BITMAP_W / 2),
        tft->height() - 64 - STATE_ERROR_BITMAP_H,
        STATE_ERROR_BITMAP,
        STATE_ERROR_BITMAP_W,
        STATE_ERROR_BITMAP_H,
        STATE_ERROR_BITMAP_COLOR
    );
}

void StateError::loop()
{
    return;
}

void StateError::exit()
{
    return;
}