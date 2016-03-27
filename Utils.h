#ifndef __CAL_UTILS__
#define __CAL_UTILS__

#include "Time.h"
#include "SPI.h"
#include <Adafruit_TFTLCD.h>
#include "TouchScreen.h"
#include <Fat16.h>

// Version info
#define VERSION1 1
#define VERSION2 1

// Touch screen settings
#define TS_MINX 200
#define TS_MINY 190
#define TS_MAXX 950
#define TS_MAXY 940

// Bitmap draw setting
#define BMP_BUFFER_SIZE 128

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

// Icons
#define ICON_BDAY 0
#define ICON_EVENT 1
#define ICON_ANNIVERSARY 2
#define ICON_ARROW_UP 3
#define ICON_ARROW_DOWN 4

class Utils
{
public:

    Adafruit_TFTLCD* tft;
    Fat16* file;

    Utils(Adafruit_TFTLCD* _tft, Fat16* _file);
    ~Utils();

    // draw functions
    void drawString(uint16_t x, uint16_t y, char* string, uint16_t fg, uint16_t bg, uint8_t size);
    void drawImage(uint16_t _x, uint16_t _y, uint8_t number, uint16_t _w, uint16_t _h);

    // touch
    void updateTouchState(int16_t x, int16_t y);
    bool isTouched(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

    // file handling
    bool fileOpen(char* filename);
    void fileClose();
    bool hasFile();
    bool fileSeek(uint32_t pos);
    uint32_t filePosition();
    uint32_t fileSize();
    char fileRead();

    // general purpose
    uint8_t daysInMonth(uint8_t _month, uint16_t _year);

private:

    bool hasTouch;
    uint16_t touchX;
    uint16_t touchY;

    uint8_t lastDay;
    uint8_t lastMinute;

    char* currentFile;

};

#endif