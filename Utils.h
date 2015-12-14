#ifndef __CAL_UTILS__
#define __CAL_UTILS__

#include <string.h>

#include "SPI.h"
#include <Adafruit_TFTLCD.h>
#include "TouchScreen.h"

// Touch screen settings
#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940
#define TS_MINPRESSURE 10
#define TS_MAXPRESSURE 1000

class Utils
{
public:
	static const uint8_t FONT_SIZE_W = 6;
	static const uint8_t FONT_SIZE_H = 8;
	static void drawString(Adafruit_TFTLCD* tft, uint16_t x, uint16_t y, char* string, uint16_t fg, uint16_t bg, uint8_t size);
    
    static bool isTouched(TouchScreen* ts, Adafruit_TFTLCD* tft, uint16_t x, uint16_t y, uint16_t w, uint16_t h);

};

// Bitmap arrow up
const uint8_t BITMAP_ARROW_UP_W = 16;
const uint8_t BITMAP_ARROW_UP_H = 16;
const uint8_t PROGMEM BITMAP_ARROW_UP [] = {
0x00, 0x00, 0x00, 0x80, 0x01, 0x80, 0x01, 0xC0, 0x03, 0xC0, 0x03, 0xE0, 0x07, 0xE0, 0x0F, 0xF0,
0x0F, 0xF0, 0x1F, 0xF8, 0x1F, 0xF8, 0x3F, 0xFC, 0x3F, 0xFC, 0x7F, 0xFC, 0x7F, 0xFE, 0xFF, 0xFE
};

// Bitmap arrow down
const uint8_t BITMAP_ARROW_DOWN_W = 16;
const uint8_t BITMAP_ARROW_DOWN_H = 16;
const uint8_t PROGMEM BITMAP_ARROW_DOWN [] = {
0xFF, 0xFE, 0x7F, 0xFE, 0x7F, 0xFC, 0x3F, 0xFC, 0x3F, 0xFC, 0x1F, 0xF8, 0x1F, 0xF8, 0x0F, 0xF0,
0x0F, 0xF0, 0x07, 0xE0, 0x03, 0xE0, 0x03, 0xC0, 0x01, 0xC0, 0x01, 0x80, 0x00, 0x80, 0x00, 0x00
};

#endif