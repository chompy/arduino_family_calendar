#ifndef __CAL_UTILS__
#define __CAL_UTILS__

#include "Time.h"
#include "SPI.h"
#include <Adafruit_TFTLCD.h>
#include "TouchScreen.h"
#include <SD.h>

// Touch screen settings
#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940
#define TS_MINPRESSURE 10
#define TS_MAXPRESSURE 1000

// Bitmap draw setting
#define BUFFPIXEL 20

// Font settings
#define FONT_SIZE_W 6
#define FONT_SIZE_H 8

// Date/time display settings
#define DISPLAY_DATETIME_TEXT_COLOR 0xFFFF
#define DISPLAY_DATETIME_BG_COLOR 0x0000
#define DISPLAY_TIME_LENGTH 8
#define DISPLAY_TIME_FONT_SIZE 2
#define DISPLAY_DATE_SPACING 4
#define DISPLAY_DATE_LENGTH 10
#define DISPLAY_DATE_FONT_SIZE 1

// Event display settings
#define DISPLAY_EVENT_COUNT 5
#define DISPLAY_EVENT_TEXT_COLOR 0xFFFF
#define DISPLAY_EVENT_BG_COLOR 0x0000
#define DISPLAY_EVENT_FONT_SIZE 1
#define DISPLAY_EVENT_LENGTH 13
#define DISPLAY_EVENT_DATE_FONT_SIZE 1
#define DISPLAY_EVENT_DATE_PADDING 3
#define DISPLAY_EVENT_PADDING 16
#define DISPLAY_EVENT_OFFSET_Y 16
#define DISPLAY_EVENT_ICON_XOFFSET 12
#define DISPLAY_EVENT_XOFFSET 25

class Utils
{
public:

    Adafruit_TFTLCD* tft;

    Utils(Adafruit_TFTLCD* _tft, TouchScreen* _ts);
    ~Utils();

    // draw functions
    void drawString(uint16_t x, uint16_t y, char* string, uint16_t fg, uint16_t bg, uint8_t size);
    void drawBitmap(char *filename, int x, int y);

    // other misc
    bool isTouched(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

private:
    TouchScreen* ts;

    uint8_t lastDay;
    uint8_t lastMinute;

    uint16_t read16(File f);
    uint32_t read32(File f);
};

#endif