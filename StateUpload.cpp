#include "StateUpload.h"

char sendString[] = "SENDING PHOTOS TO PC";
char recvString[] = "RECEIVING PHOTOS FROM PC";
char photoFilename[] = "photos.dat";

StateUpload::~StateUpload()
{

}

void StateUpload::enter()
{
    state = SP_STATE_WAIT;
    displayWait();
}

void StateUpload::loop()
{
    if (Serial.available() > 0) {
        switch (state) {
            case SP_STATE_WAIT:
            {
                uint8_t byte = Serial.read();
                switch (byte) {
                    case SP_STATE_PHOTO_SEND:
                    {
                        if (!utils->fileOpen(photoFilename)) {
                            break;
                        }

                        utils->tft->fillScreen(0x0000);
                        utils->drawString(
                            (utils->tft->width() / 2) - (((FONT_SIZE_W * STATE_UPLOAD_TEXT_SIZE) * strlen(sendString)) / 2),
                            (utils->tft->height() / 2) - (((FONT_SIZE_H * STATE_UPLOAD_TEXT_SIZE)) / 2),
                            sendString,
                            STATE_UPLOAD_TEXT_COLOR,
                            STATE_UPLOAD_BG_COLOR,
                            STATE_UPLOAD_TEXT_SIZE
                        );

                        uint8_t buf[SEND_BUF_SIZE];
                        while(true) {
                            int16_t amount = utils->file->read(buf, SEND_BUF_SIZE);
                            if (amount <= 0) {
                                break;
                            }
                            Serial.write( buf, SEND_BUF_SIZE );
                        }
                        utils->fileClose();

                        displayWait();
                        break;
                    }
                    case SP_STATE_PHOTO_RECV:
                    {

                        utils->tft->fillScreen(0x0000);
                        utils->drawString(
                            (utils->tft->width() / 2) - (((FONT_SIZE_W * STATE_UPLOAD_TEXT_SIZE) * strlen(recvString)) / 2),
                            (utils->tft->height() / 2) - (((FONT_SIZE_H * STATE_UPLOAD_TEXT_SIZE)) / 2),
                            recvString,
                            STATE_UPLOAD_TEXT_COLOR,
                            STATE_UPLOAD_BG_COLOR,
                            STATE_UPLOAD_TEXT_SIZE
                        );

                        if (!utils->file->open(photoFilename, O_WRITE)) {
                            break;
                        }

                        state = SP_STATE_PHOTO_RECV;
                        photoDownloadSize = 0;
                        uint8_t buf[4];
                        Serial.readBytes(buf, 4);
                        memcpy(&photoDownloadSize, &buf, 4);

                        displayWait();
                        break;

                    }
                }
                break;
            }
            case SP_STATE_PHOTO_RECV:
            {
                char buf[Serial.available()];
                uint8_t readBytes = Serial.readBytes(buf, Serial.available());
                if (readBytes > 0) {
                    utils->file->write(buf, readBytes);
                }
                if (utils->file->curPosition() >= photoDownloadSize) {
                    state = SP_STATE_WAIT;
                    utils->file->close();
                    displayWait();
                }
                break;
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