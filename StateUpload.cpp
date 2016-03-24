#include "StateUpload.h"

StateUpload::~StateUpload()
{

}

void StateUpload::enter()
{
    state = SP_STATE_WAIT;
    photoRequest = 0;
    char uploadString[] = "WAITING FOR DATA";
    utils->drawString(
        (utils->tft->width() / 2) - (((FONT_SIZE_W * STATE_UPLOAD_TEXT_SIZE) * 16) / 2),
        (utils->tft->height() / 2) - (((FONT_SIZE_H * STATE_UPLOAD_TEXT_SIZE)) / 2),
        uploadString,
        STATE_UPLOAD_TEXT_COLOR,
        STATE_UPLOAD_BG_COLOR,
        STATE_UPLOAD_TEXT_SIZE
    );
}

void StateUpload::loop()
{
    if (Serial.available() > 0) {

        uint8_t byte = Serial.read();

        switch (state) {
            case SP_STATE_WAIT:
            {
                switch (byte) {
                    case SP_STATE_PHOTO_COUNT:
                    {
                        uint8_t photoCount = 0;
                        if (utils->fileOpen("photos.dat")) {
                            photoCount = (uint8_t) utils->fileRead();
                            utils->fileClose();
                        }
                        Serial.write( photoCount );
                        break;
                    }
                    case SP_STATE_PHOTO_REQUEST:
                    {

                        char uploadString[] = "SENDING PHOTO";
                        utils->drawString(
                            (utils->tft->width() / 2) - (((FONT_SIZE_W * STATE_UPLOAD_TEXT_SIZE) * 16) / 2),
                            (utils->tft->height() / 2) - (((FONT_SIZE_H * STATE_UPLOAD_TEXT_SIZE)) / 2),
                            uploadString,
                            STATE_UPLOAD_TEXT_COLOR,
                            STATE_UPLOAD_BG_COLOR,
                            STATE_UPLOAD_TEXT_SIZE
                        );

                        while (Serial.available() < 1) {
                            delay(100);
                        }
                        photoRequest = (uint8_t) Serial.read();

                        utils->fileOpen("photos.dat");
                        uint16_t photoSize[2];
                        utils->imageSize(photoRequest, photoSize);
                        utils->fileClose();
                        Serial.write(photoSize[0]);
                        Serial.write(photoSize[1]);

                        break;
                    }
                }

            }
        }

    }
}

void StateUpload::exit()
{

}