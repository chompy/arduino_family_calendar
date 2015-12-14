#include "Utils.h"

void Utils::drawString(Adafruit_TFTLCD* tft, uint16_t x, uint16_t y, char* string, uint16_t fg, uint16_t bg, uint8_t size)
{
    for (unsigned int i = 0; i < strlen(string); i++) {
        tft->drawChar(
            x,
            y,
            string[i],
            fg,
            bg,
            size
        );
        x += Utils::FONT_SIZE_W * size;
    }
}

bool Utils::isTouched(TouchScreen* ts, Adafruit_TFTLCD* tft, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    TSPoint p = ts->getPoint();
    if (!(p.z > TS_MINPRESSURE && p.z < TS_MAXPRESSURE)) {
        return false;
    }
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft->width());
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft->width());

    Serial.println( p.x );
    Serial.println( p.y );
    return false;
}