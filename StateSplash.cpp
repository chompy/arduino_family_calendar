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
            0
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