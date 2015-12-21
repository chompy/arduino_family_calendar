#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <SPI.h>
#include <SD.h>
#include <TouchScreen.h>
#include "Utils.h"

#include "State.h"
#include "StateMain.h"
#include "StateSetClock.h"
#include "StateError.h"

// Touch Settings
#define YP A2
#define XM A3
#define YM 8
#define XP 9

TouchScreen ts(XP, YP, XM, YM, 300);

// TFT Settings
#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0 
#define LCD_RESET A4
#define ROTATION 3

//TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

// Color definitions
#define BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0 
#define WHITE           0xFFFF

// Default time
#define TIME_DEFAULT_HOUR 1
#define TIME_DEFAULT_MINUTE 12
#define TIME_DEFAULT_MONTH 12
#define TIME_DEFAULT_DAY 20
#define TIME_DEFAULT_YEAR 2015

// Vars
uint8_t photoCount = 0;
time_t lastPhotoChange = 0;
uint8_t currentPhoto = 0;

// Utility Class
Utils utils(&tft, &ts);

// init function
void setup(void) {

    // init
    Serial.begin(9600);
    tft.reset();
    tft.begin(); 
    tft.fillScreen(BLACK);
    tft.setRotation(ROTATION);
    //pinMode(13, OUTPUT);

    // time in 12 hour format
    setTime(
        TIME_DEFAULT_HOUR,
        TIME_DEFAULT_MINUTE,
        0,
        TIME_DEFAULT_DAY,
        TIME_DEFAULT_MONTH,
        TIME_DEFAULT_YEAR
    );
    randomSeed(now());

    // boot sd
    if (!SD.begin()) {
        StateError::setMessage("SD Card failed, or not present");
        State::changeState(
            StateError::ID
        );
        return;
    }

    // goto state
    State::changeState(
        StateSetClock::ID
    );

}

// main loop
void loop()
{

    // new state
    if (State::hasNextState()) {
        // states
        switch(State::getNextState())
        {
            case StateError::ID:
            {
                State::changeState(
                    new StateError(
                        &utils
                    )
                );
                break;
            }
            case StateMain::ID:
            {
                State::changeState(
                    new StateMain(
                        &utils
                    )
                );
                break;
            }
            case StateSetClock::ID:
            {
                State::changeState(
                    new StateSetClock(
                        &utils
                    )
                );
                break;
            }        
        }        
    }

    // state loop
    State::loopCurrent();

    // 1/5 second
    delay(200);

}
