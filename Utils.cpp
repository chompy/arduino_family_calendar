#include "Utils.h"

Utils::Utils(Adafruit_TFTLCD* _tft)
{
    tft = _tft;

    hasTouch = false;
    touchX = 0;
    touchY = 0;

    lastMinute = 99;
    lastDay = 99;

    currentFile = nullptr;
    _filePosition = 0;

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
void Utils::drawImage(uint16_t _x, uint16_t _y, uint8_t number)
{
    if (!hasFile()) {
        return;
    }

    uint32_t photoSize[2];

    for (uint8_t i = 0; i <= number; i++) {

        // get size of photo (4 bytes)
        for (uint8_t j = 0; j < 2; j++) {
            uint8_t bytes[2];
            bytes[0] = fileRead();
            bytes[1] = fileRead();
            photoSize[j] = 0;
            photoSize[j] = (photoSize[j] << 8) + bytes[1];
            photoSize[j] = (photoSize[j] << 8) + bytes[0];
        }

        // is image to draw
        if (i == number) {
            break;
        }

        // next
        fileSeek(filePosition() + ((photoSize[0] * photoSize[1]) * 2));
    }

    // define image draw space
    tft->setAddrWindow(
        _x,
        _y,
        _x + photoSize[0] - 1,
        _y + photoSize[1] - 1
    );

    // iterate file, draw pixel dataj
    uint32_t endPos = filePosition() + ((photoSize[0] * photoSize[1]) * 2);
    bool isFirst = true;
    while(filePosition() < endPos) {

        uint16_t amount = 510;
        if (512 - filePosition() % 512 < 510) {
            amount = 512 - filePosition() % 512;

            if (amount % 2 != 0) {
                amount -= 1;
                if (amount == 0) {
                    uint8_t bytes[2];
                    bytes[0] = fileRead();
                    bytes[1] = fileRead();
                    tft->pushColors(
                        (uint16_t*) &bytes,
                        1,
                        false
                    );
                    continue;
                }
            }
        }
        tft->pushColors(
            (uint16_t*) &file.buffer[ filePosition() % 512 ],
            endPos - filePosition() < 510 ? (endPos - filePosition()) / 2 : amount / 2,
            isFirst
        );
        isFirst = false;
        fileSeek(filePosition() + amount);
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
    if (file.openFile(filename, FILEMODE_BINARY) == NO_ERROR) {
        _filePosition = 0;
        word res = file.readBinary();
        if (res <= 0 || res > 512) {
            file.closeFile();
            return false;
        }

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
    file.closeFile();
    _filePosition = 0;
}

bool Utils::hasFile()
{
    if (currentFile) {
        return true;
    }
    return false;
}

bool Utils::fileSeek(uint32_t pos)
{
    if (!hasFile()) {
        return false;
    }
    if ( pos / 512 == _filePosition / 512 ) {
        _filePosition = pos;
        return true;
    
    } else if ( pos > _filePosition ) {

        for (uint8_t i = 0; i < (pos / 512) - (_filePosition / 512); i++) {
            word res = file.readBinary();
            if (res <= 0 || res > 512) {
                return false;
            }
            if (res < 512) {
                if ( i > 0 && i < (pos / 512) - (_filePosition / 512) - 1 ) {
                    _filePosition = ((i - 1) * 512) + i;
                    break;
                }
                break;
            }
        }
        _filePosition = pos;
        return true;
    } else {
        char reopenFile[strlen(currentFile)];
        strcpy(reopenFile, currentFile);

        if (!fileOpen(reopenFile)) {
            return false;
        }
        return fileSeek(pos);
    }
    return false;
}

uint32_t Utils::filePosition()
{
    return _filePosition;
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
    char next = file.buffer[ _filePosition % 512 ];
    fileSeek(_filePosition + 1);
    return next;
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