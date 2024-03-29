#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <SPI.h>
#include <Fat16.h>
#include <Fat16util.h>

#include <TouchScreen.h>
#include "Utils.h"

#include "State.h"
#include "StateMain.h"
#include "StateSetClock.h"
#include "StateError.h"
#include "StateSplash.h"
#include "StateUpload.h"

// Touch Settings
#define YP A2
#define XM A3
#define YM 8
#define XP 9
#define TS_MINPRESSURE 10
#define TS_MAXPRESSURE 1000

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
#define TIME_DEFAULT_HOUR 12
#define TIME_DEFAULT_MINUTE 0
#define TIME_DEFAULT_MONTH 3
#define TIME_DEFAULT_DAY 27
#define TIME_DEFAULT_YEAR 2016

// sd card
SdCard card;
Fat16 file;

// Utility Class
Utils utils(&tft, &file);

bool hasSerial = false;

// init function
void setup(void) {

    // init
    Serial.begin(57600);
    tft.reset();
    tft.begin(); 
    tft.fillScreen(BLACK);
    tft.setRotation(ROTATION);

    // set time
    setTime(
        TIME_DEFAULT_HOUR,
        TIME_DEFAULT_MINUTE,
        0,
        TIME_DEFAULT_DAY,
        TIME_DEFAULT_MONTH,
        TIME_DEFAULT_YEAR
    );
    randomSeed(now());

    // initialize the SD card
    if (!card.init()) {
        StateError::setMessage("SD Card failed, or not present (err 1)");
        State::changeState(
            StateError::ID
        );
        return;        
    }
  
    // initialize a FAT16 volume
    if (!Fat16::init(&card)) {
        StateError::setMessage("SD Card failed, or not present (err 2)");
        State::changeState(
            StateError::ID
        );
        return;    
    }

    State::changeState(
        StateSplash::ID
    );

}

// main loop
void loop()
{

    // switch to upload mode if serial data connection is received
    if (!hasSerial && Serial.available() > 0) {
        State::changeState(
            StateUpload::ID
        );
        hasSerial = true;
    }

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
            case StateSplash::ID:
            {
                State::changeState(
                    new StateSplash(
                        &utils
                    )
                );
                break;
            }
            case StateUpload::ID:
            {
                State::changeState(
                    new StateUpload(
                        &utils
                    )
                );
            }
        }        
    }

    // state loop
    State::loopCurrent();

    // read touch status
    uint16_t pressure = ts.pressure();
    if (pressure > TS_MINPRESSURE && pressure < TS_MAXPRESSURE) {
        utils.updateTouchState( ts.readTouchX(), ts.readTouchY() );  
    } else {
        utils.updateTouchState( -1, -1 );  
    }

    // if sharing pins, you'll need to fix the directions of the touchscreen pins
    pinMode(XP, OUTPUT);
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    pinMode(YM, OUTPUT);

    // 1/5 second
    delay(200);

}
