#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <SPI.h>
#include <SD.h>
#include <TouchScreen.h>
#include <Time.h>

#include "State.h"
#include "StateError.h"
#include "StateSetClock.h"
#include "StateMain.h"

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
#define ROTATION 1

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
#define TIME_DEFAULT_HOUR 23
#define TIME_DEFAULT_MINUTE 59
#define TIME_DEFAULT_MONTH 1
#define TIME_DEFAULT_DAY 1
#define TIME_DEFAULT_YEAR 2016

void setup(void) {

    // init
    Serial.begin(9600);
    tft.reset();
    tft.begin(0x9341); 
    tft.fillScreen(BLUE);
    tft.setRotation(ROTATION);
    pinMode(13, OUTPUT);

    // time in 12 hour format
    setTime(
        TIME_DEFAULT_HOUR,
        TIME_DEFAULT_MINUTE,
        0,
        TIME_DEFAULT_DAY,
        TIME_DEFAULT_MONTH,
        TIME_DEFAULT_YEAR
    );    

    // boot sd
    if (!SD.begin()) {
        Serial.println("SD Card failed, or not present");

        State::changeState(
            new StateError(
                &tft,
                "SD Card failed, or not present"
            )
        );

        return;
    }

    // enter state 'SetClock'
    State::changeState(
        new StateMain(
            &tft
        )
    );

    //Serial.println( SD.exists("events/bday_nso.txt") ? "HAS" : "NARP" );
}

void loop()
{

    State::loopCurrent();

    /*char timeStr[8];
    sprintf(
        timeStr,
        "%d:%d:%d",
        hour(),
        minute(),
        second()
    );

    tft.fillRect(32, 32, 240, 16, BLUE);
    tft.fillRect(32, 64, 240, 16, BLUE);

    tft.drawString(
        32,
        32,
        timeStr,
        WHITE,
        2
    );

    digitalWrite(13, HIGH);
    Point p = ts.getPoint();
    digitalWrite(13, LOW);

    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    pinMode(YM, OUTPUT);
    */

    // 1/15 second
    delay(67);

}
