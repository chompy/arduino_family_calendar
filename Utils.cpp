/*
This file is part of "arduino_family_calendar".

"arduino_family_calendar" is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"arduino_family_calendar" is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "arduino_family_calendar".  If not, see <https://www.gnu.org/licenses/>.
*/

#include "Utils.h"

Utils::Utils(Adafruit_TFTLCD* _tft, Fat16* _file)
{
    tft = _tft;
    file = _file;

    hasTouch = false;
    touchX = 0;
    touchY = 0;

    lastMinute = 99;
    lastDay = 99;

    currentFile = nullptr;

}

Utils::~Utils()
{

}

void Utils::drawString(uint16_t x, uint16_t y, char* string, uint16_t fg, uint16_t bg, uint8_t size)
{
    uint8_t i = 0;
    while (string[i] != '\0') {
        tft->drawChar(
            x,
            y,
            string[i],
            fg,
            bg,
            size
        );
        x += FONT_SIZE_W * size;
        i++;
    }
}

// draws given image number from image sequence data 
// contained in currently opened file
void Utils::drawImage(uint16_t _x, uint16_t _y, uint8_t number, uint16_t _w = 0, uint16_t _h = 0)
{
    if (!hasFile()) {
        return;
    }

    file->seekSet(1);

    uint16_t photoSize[2];

    for (uint8_t i = 0; i <= number; i++) {

        // get size of photo (4 bytes)
        file->read(&photoSize, 4);

        // is image to draw
        if (i == number) {
            break;
        }

        // next
        file->seekCur( (( (uint32_t) photoSize[0] * (uint32_t) photoSize[1]) * 2) );
    }

    // centering
    if (_w > 0 && photoSize[0] < _w) {
        _x = _x + (_w / 2) - (photoSize[0] / 2);
    }
    if (_h > 0 && photoSize[1] < _h) {
        _y = _y + (_h / 2) - (photoSize[1] / 2);
    }

    // define image draw space
    tft->setAddrWindow(
        _x,
        _y,
        _x + photoSize[0] - 1,
        _y + photoSize[1] - 1
    );

    // iterate file, draw pixel data
    uint32_t endPos = filePosition() + (((uint32_t) photoSize[0] * (uint32_t) photoSize[1]) * 2);
    bool isFirst = true;

    uint16_t buf[BMP_BUFFER_SIZE / 2];
    int16_t amount = 0;

    while(filePosition() < endPos) {
        amount = file->read(buf, BMP_BUFFER_SIZE);
        if (amount <= 0) {
            break;
        }
        tft->pushColors(
            buf,
            BMP_BUFFER_SIZE / 2,
            isFirst
        );
        isFirst = false;
    }

}

void Utils::updateTouchState(int16_t x, int16_t y)
{
    hasTouch = false;
    if (x < 0 || y < 0) {
        return;
    }
    hasTouch = true;
    touchX = (uint16_t) map(x, TS_MINX, TS_MAXX, tft->width(), 0);
    touchY = (uint16_t) map(y, TS_MINY, TS_MAXY, tft->height(), 0);
}

bool Utils::isTouched(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    if (!hasTouch) {
        return false;
    }
    if (
        touchX > x && 
        touchX < x + w &&
        touchY > y &&
        touchY < y + h
    ) {
        return true;
    }
    return false;
}

bool Utils::fileOpen(char* filename)
{
    if (hasFile()) {
        fileClose();
    }
    if (file->open(filename, O_READ)) {
        file->rewind();
        currentFile = new char[strlen(filename)];
        strcpy(currentFile, filename);
        return true;
    }
    return false;
}

void Utils::fileClose()
{
    if (!hasFile()) {
        return;
    }
    delete currentFile;
    currentFile = nullptr;
    file->close();
}

bool Utils::hasFile()
{
    return (bool) currentFile && file->isOpen();
}

bool Utils::fileSeek(uint32_t pos)
{
    if (!hasFile()) {
        return false;
    }
    return file->seekSet(pos);
}

uint32_t Utils::filePosition()
{
    return file->curPosition();
}

uint32_t Utils::fileSize()
{
    return 0;
}

char Utils::fileRead()
{
    if (!hasFile()) {
        return '\0';
    }
    int16_t byte = file->read();
    return byte > 0 ? (char) byte : '\0';
}

uint8_t Utils::daysInMonth(uint8_t _month, uint16_t _year)
{
    // april, june, september, november
    if ( _month == 4 || _month == 6 || _month == 9 || _month == 11 ) {
        return 30;

    // feburary
    } else if (_month == 2) {

        // leap year
        if ( 
            (_year % 4 == 0 && _year % 100 != 0) || (_year % 400 == 0)
         ) {
            return 29;
        }

        // normal year
        return 28;

    } 

    // the rest
    return 31;
}