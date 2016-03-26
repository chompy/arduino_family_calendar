#include "StateUpload.h"

StateUpload::~StateUpload()
{

}

void StateUpload::enter()
{
    state = SP_STATE_WAIT;
    photoRequest = 0;
    displayWait();
}

void StateUpload::loop()
{
    if (Serial.available() > 0) {

        uint8_t byte = Serial.read();

        switch (state) {
            case SP_STATE_WAIT:
            {
                switch (byte) {
                    case SP_STATE_PHOTO_SEND:
                    {
                        if (!utils->fileOpen("photos.dat")) {
                            break;
                        }

                        utils->tft->fillScreen(0x0000);
                        char uploadString[] = "SENDING PHOTOS TO PC";
                        utils->drawString(
                            (utils->tft->width() / 2) - (((FONT_SIZE_W * STATE_UPLOAD_TEXT_SIZE) * strlen(uploadString)) / 2),
                            (utils->tft->height() / 2) - (((FONT_SIZE_H * STATE_UPLOAD_TEXT_SIZE)) / 2),
                            uploadString,
                            STATE_UPLOAD_TEXT_COLOR,
                            STATE_UPLOAD_BG_COLOR,
                            STATE_UPLOAD_TEXT_SIZE
                        );

                        uint32_t pos = 0;
                        while(utils->fileSeek(pos)) {
                            Serial.write( file.buffer, 512 );
                            pos += 512;
                        }
                        utils->fileClose();

                        displayWait();
                        break;
                    }
                    case SP_STATE_PHOTO_RECV:
                    {

                        utils->tft->fillScreen(0x0000);
                        char uploadString[] = "RECEIVING PHOTOS FROM PC";

                        char photoFilename[] = "photos.dat";
                        utils->drawString(
                            (utils->tft->width() / 2) - (((FONT_SIZE_W * STATE_UPLOAD_TEXT_SIZE) * strlen(uploadString)) / 2),
                            (utils->tft->height() / 2) - (((FONT_SIZE_H * STATE_UPLOAD_TEXT_SIZE)) / 2),
                            uploadString,
                            STATE_UPLOAD_TEXT_COLOR,
                            STATE_UPLOAD_BG_COLOR,
                            STATE_UPLOAD_TEXT_SIZE
                        );

                        if (file.exists(photoFilename)) {
                            file.delFile(photoFilename);
                        }
                        file.create(photoFilename);


                        displayWait();
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

void StateUpload::displayWait()
{
    utils->tft->fillScreen(0x0000);
    char uploadString[] = "WAITING FOR PC";
    utils->drawString(
        (utils->tft->width() / 2) - (((FONT_SIZE_W * STATE_UPLOAD_TEXT_SIZE) * strlen(uploadString)) / 2),
        (utils->tft->height() / 2) - (((FONT_SIZE_H * STATE_UPLOAD_TEXT_SIZE)) / 2),
        uploadString,
        STATE_UPLOAD_TEXT_COLOR,
        STATE_UPLOAD_BG_COLOR,
        STATE_UPLOAD_TEXT_SIZE
    );
}