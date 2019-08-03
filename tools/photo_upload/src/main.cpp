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

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include "SerialClass.h"

#define MENU_SIZE 36

#define PHOTO_W 200
#define PHOTO_H 240
#define PHOTO_PAD 16

#define SP_STATE_NONE 0
#define SP_STATE_WAIT 1
#define SP_STATE_PHOTO_SEND 2
#define SP_STATE_PHOTO_RECV 3

int screenWidth = (3 * (PHOTO_W + PHOTO_PAD)) + MENU_SIZE + PHOTO_PAD;
int screenHeight = (2 * (PHOTO_H + PHOTO_PAD)) + MENU_SIZE + (PHOTO_PAD * 2);

struct PhotoData
{
    SDL_Texture* texture;
    int position;
};
std::vector<PhotoData> photos;

int viewOffset = 0;

int spState = 0;
unsigned char devicePhotoCount = 255;
unsigned long devicePhotoPos = 0;
unsigned int devicePhotoW = 0;
unsigned int devicePhotoH = 0;
std::vector<char> photoData;

SDL_Renderer* renderer = nullptr;
SDL_Window* window = nullptr;
SDL_Texture* loadingIcon = nullptr;
SDL_Texture* uploadButton = nullptr;
SDL_Texture* displayText = nullptr;
int textW, textH;

SDL_Rect uploadButtonRect;


void drawLoading()
{
    SDL_RenderSetClipRect(renderer, NULL);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderFillRect(renderer, NULL);
    SDL_Rect loadingRect;
    SDL_QueryTexture(loadingIcon, NULL, NULL, &loadingRect.w, &loadingRect.h);
    loadingRect.x = (screenWidth / 2) - (loadingRect.w / 2);
    loadingRect.y = (screenHeight / 2) - (loadingRect.h / 2);
    SDL_RenderCopy(
        renderer,
        loadingIcon,
        NULL,
        &loadingRect
    );
    SDL_RenderPresent(renderer);
}

int main(int argc, char **argv) {

    SDL_Init(SDL_INIT_EVENTS);
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
        "FamCal Photo Tool",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        screenWidth,
        screenHeight,
        SDL_WINDOW_SHOWN |
        SDL_WINDOW_RESIZABLE
    );
    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    // "loading" image
    SDL_Surface* loadingIconSurface = IMG_Load("assets/loading.png");
    if (loadingIconSurface) {
        loadingIcon = SDL_CreateTextureFromSurface(renderer, loadingIconSurface);
        SDL_FreeSurface(loadingIconSurface);
    }

    // "upload" button
    SDL_Surface* uploadButtonSurface = IMG_Load("assets/btn_upload.png");
    if (uploadButtonSurface) {
        uploadButton = SDL_CreateTextureFromSurface(renderer, uploadButtonSurface);
        SDL_FreeSurface(uploadButtonSurface);
    }
    uploadButtonRect.x = 8;
    uploadButtonRect.y = (MENU_SIZE / 2) - (14);
    uploadButtonRect.w = 128;
    uploadButtonRect.h = 28;

    // main font
    TTF_Init();
    TTF_Font* font = TTF_OpenFont("assets/font.ttf", 16);

    SDL_SetMainReady();

    drawLoading();

    // connect to arduino
    Serial* SP = nullptr;
    Serial* trySerialPorts[8];
    for (int i = 2; i < 10; i++) {
        char comPort[16];
        sprintf(comPort, "\\\\.\\COM%d", i);
        trySerialPorts[i] = new Serial(comPort);
    }
    SDL_Delay(4000);
    for (int i = 2; i < 10; i++) {
        if (trySerialPorts[i]->IsConnected()) {
            SP = trySerialPorts[i];
            std::cout << "* Connected to device on COM" << i << "." << std::endl;
            SDL_Delay(500);
            break;
        }
    }
    for (int i = 2; i < 10; i++) {
        if (SP != trySerialPorts[i]) {
            delete trySerialPorts[i];
        }
    }

    bool loop = true;

    if (!SP) {
        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            "Error",
            "Could not connect to FamCal device.",
            window
        );
        loop = false;
    }   

    SDL_Event e;
    while(loop)
    {
        while( SDL_PollEvent(&e) ) {
            switch (e.type) {
                case SDL_QUIT:
                {
                    loop = false;
                    break;
                }
                case SDL_WINDOWEVENT:
                {
                    switch(e.window.event) {
                        case SDL_WINDOWEVENT_RESIZED:
                        {
                            screenWidth = e.window.data1;
                            screenHeight = e.window.data2;
                            break;
                        }
                    }
                    break;
                }
                case SDL_DROPFILE:
                {

                    if (spState != SP_STATE_WAIT && spState != SP_STATE_NONE) {
                        break;
                    }

                    drawLoading();

                    char* filename = e.drop.file;
                    SDL_Surface* photoSurface = IMG_Load(filename);
                    if (!photoSurface) {
                        SDL_ShowSimpleMessageBox(
                            SDL_MESSAGEBOX_ERROR,
                            "Error",
                            "File is not a valid image.",
                            window
                        );
                    } else {

                        SDL_Surface* scaledSurface = SDL_CreateRGBSurface(
                            0,
                            PHOTO_W,
                            PHOTO_H,
                            16,
                            0xF800, 0x7E0, 0x1F, 0
                        );

                        SDL_Rect dest;
                        dest.x = 0;
                        dest.y = 0;

                        if (photoSurface->w > photoSurface->h) {
                            dest.w = PHOTO_W;
                            dest.h = photoSurface->h / (photoSurface->w / PHOTO_W);
                            dest.y = (PHOTO_H / 2) - (dest.h / 2);

                        } else {
                            dest.w = photoSurface->w / (photoSurface->h / PHOTO_H);
                            dest.h = PHOTO_H;
                            dest.x = (PHOTO_W / 2) - (dest.w / 2);
                        }

                        SDL_BlitScaled(
                            photoSurface,
                            NULL,
                            scaledSurface,
                            &dest
                        );
                        SDL_FreeSurface(photoSurface);
                        PhotoData pd;
                        pd.position = 1;
                        pd.texture = SDL_CreateTextureFromSurface(
                            renderer,
                            scaledSurface
                        );
                        SDL_FreeSurface(scaledSurface);
                        photos.push_back(pd);
                    }
                    SDL_free(filename);
                    viewOffset = 10000;

                    break;
                }
                case SDL_MOUSEWHEEL:
                {
                    viewOffset += e.wheel.y * -15;
                    break;
                }
                case SDL_MOUSEBUTTONDOWN:
                {
                    SDL_Point mousePos;
                    mousePos.x = e.button.x;
                    mousePos.y = e.button.y;

                    if (SDL_PointInRect(&mousePos, &uploadButtonRect)) {
                        spState = SP_STATE_PHOTO_SEND;
                    }

                    break;
                }
            }
        }

        // lost connection
        if (!SP->IsConnected()) {
            SDL_ShowSimpleMessageBox(
                SDL_MESSAGEBOX_ERROR,
                "Error",
                "Lost connection to FamCal device.",
                window
            );
            loop = false;
        }

        // serial transfer
        switch (spState) {

            // at startup request photos
            case SP_STATE_NONE:
            {
                std::cout << "* Connected! Requesting photos." << std::endl;
                spState = SP_STATE_PHOTO_RECV;
                char state = SP_STATE_PHOTO_SEND;
                SP->WriteData(&state, 1);
                break;
            }

            // recieve photo data
            case SP_STATE_PHOTO_RECV:
            {

                // gray out buttons
                SDL_SetTextureAlphaMod(uploadButton, 128);

                char buf[512];
                int results = SP->ReadData(buf, 512);
                if (results > 0) {
                    if (photoData.size() == 0) {
                        devicePhotoCount = buf[0];
                        std::cout << "* Device photo count is " << (int) devicePhotoCount << "." << std::endl;
                        devicePhotoPos = 1;
                    }
                    for (int i = 0; i < results; i++) {
                        photoData.push_back(buf[i]);
                    }

                    if (devicePhotoW == 0 && devicePhotoH == 0 && photoData.size() - devicePhotoPos > 4) {
                        memcpy( &devicePhotoW, &photoData[devicePhotoPos], 2);
                        memcpy( &devicePhotoH, &photoData[devicePhotoPos + 2], 2);

                        if (font) {
                            char downloadText[64];
                            sprintf(downloadText, "Downloading photo %d of %d.", (photos.size() + 1), (int) devicePhotoCount);
                            SDL_Color color = {255, 255, 255};
                            SDL_Surface* textSurface = TTF_RenderText_Solid(font, downloadText, color);
                            if (textSurface) {
                                textW = textSurface->w;
                                textH = textSurface->h;                                
                                displayText = SDL_CreateTextureFromSurface(
                                    renderer,
                                    textSurface
                                );
                                SDL_FreeSurface(textSurface);
                            }
                        }

                    } else if (devicePhotoW > 0 && devicePhotoH > 0 && photoData.size() - devicePhotoPos > 4 + (( devicePhotoW * devicePhotoH ) * 2)) {
                        std::cout << "* Got photo " << (photos.size() + 1) << " of " << (int) devicePhotoCount << " (" << devicePhotoW << "x" << devicePhotoH << ")." << std::endl;
                        PhotoData pd;
                        pd.position = 0;
                        SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(
                            &photoData[devicePhotoPos + 4],
                            devicePhotoW,
                            devicePhotoH,
                            16,
                            devicePhotoW * 2,
                            0xF800, 0x7E0, 0x1F, 0
                        );
                        pd.texture = SDL_CreateTextureFromSurface(
                            renderer,
                            surface
                        );
                        SDL_FreeSurface(surface);
                        photos.push_back(pd);

                        devicePhotoPos += (4 + (( devicePhotoW * devicePhotoH ) * 2));
                        devicePhotoW = 0;
                        devicePhotoH = 0;
                    }
                }

                if (photos.size() >= devicePhotoCount) {
                    spState = SP_STATE_WAIT;

                    // enable buttons
                    SDL_SetTextureAlphaMod(uploadButton, 255);

                    break;
                }

                break;
            }

            // send photo data
            case SP_STATE_PHOTO_SEND:
            {

                drawLoading();

                unsigned long fileSize = 1;
                for (unsigned int i = 0; i < photos.size(); i++) {
                    int w, h;
                    SDL_QueryTexture(photos[i].texture, NULL, NULL, &w, &h);
                    fileSize += 4 + ((w * h) * 2);
                }

                std::cout << "* Sending photos to device (" << fileSize << " bytes)." << std::endl;

                char buf[64];
                unsigned char bufPos = 0;
                buf[0] = SP_STATE_PHOTO_RECV;
                memcpy(&buf[1], &fileSize, 4);
                SP->WriteData(buf, 5);
                SDL_Delay(1000);


                buf[0] = photos.size();
                bufPos += 1;

                char* pixels = nullptr;
                int pitch = 0;
                for (unsigned int i = 0; i < photos.size(); i++) {

                    std::cout << "* Sending photo " << (i + 1) << " of " << photos.size() << "." << std::endl;

                    int w, h;
                    SDL_QueryTexture(photos[i].texture, NULL, NULL, &w, &h);
                    SDL_LockTexture(photos[i].texture, NULL, reinterpret_cast<void **>(&pixels), &pitch);

                    memcpy(&buf[bufPos], &w, 2);
                    memcpy(&buf[bufPos + 2], &h, 2);
                    bufPos += 4;

                    long photoPos = 0;
                    while(photoPos < (w * h) * 2) {
                        memcpy(&buf[bufPos], &pixels[photoPos], 64 - bufPos);
                        photoPos += 64 - bufPos;
                        SP->WriteData(buf, 64);
                        bufPos = 0;
                    }

                    SDL_UnlockTexture(photos[i].texture);
                }

                spState = SP_STATE_WAIT;

                break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer, 0, MENU_SIZE, screenWidth - MENU_SIZE, MENU_SIZE);
        SDL_RenderDrawLine(renderer, screenWidth - MENU_SIZE, MENU_SIZE, screenWidth - MENU_SIZE, screenHeight);

        SDL_RenderCopy(
            renderer,
            uploadButton,
            NULL,
            &uploadButtonRect
        );

        int availableWidth = screenWidth - MENU_SIZE;
        int availableHeight = screenHeight - MENU_SIZE;
        int imagesPerRow = availableWidth / (PHOTO_W + PHOTO_PAD);
        int totalHeight = photos.size() > 0 ? (( (photos.size() - 1) / imagesPerRow) * (PHOTO_H + PHOTO_PAD)) + (PHOTO_H + PHOTO_PAD) : 0;

        if (totalHeight < availableHeight) {
            viewOffset = 0;
        } else if (viewOffset > totalHeight - availableHeight + PHOTO_PAD) {
            viewOffset = totalHeight - availableHeight + PHOTO_PAD;
        } else if (viewOffset < 0) {
            viewOffset = 0;
        }

        SDL_Rect renderClip;
        renderClip.x = 0;
        renderClip.y = MENU_SIZE + PHOTO_PAD;
        renderClip.w = availableWidth;
        renderClip.h = availableHeight;
        SDL_RenderSetClipRect(renderer, &renderClip);

        SDL_Rect photoRect;
        photoRect.w = PHOTO_W;
        photoRect.h = PHOTO_H;

        for (unsigned int i = 0; i < photos.size(); i++) {
            photoRect.x = PHOTO_PAD + ((i % imagesPerRow) * (PHOTO_W + PHOTO_PAD));
            photoRect.y = MENU_SIZE + PHOTO_PAD + ( ((i / imagesPerRow) * (PHOTO_H + PHOTO_PAD)) - viewOffset );
            if (photos[i].texture) {
                SDL_RenderCopy(
                    renderer,
                    photos[i].texture,
                    NULL,
                    &photoRect
                );
            }

            SDL_RenderDrawRect(
                renderer,
                &photoRect
            );
        }
        
        // recieve/upload photo dialog
        if (displayText && spState == SP_STATE_PHOTO_RECV) {

            SDL_RenderSetClipRect(renderer, NULL);

            SDL_Rect dest;
            dest.w = textW + (MENU_SIZE * 2);
            dest.h = textH + (MENU_SIZE * 2);
            dest.x = (screenWidth / 2) - (dest.w / 2);
            dest.y = (screenHeight / 2) - (dest.h / 2);

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderFillRect(
                renderer,
                &dest
            );
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(
                renderer,
                &dest
            );
            
            dest.x = (screenWidth / 2) - (textW / 2);
            dest.y = (screenHeight / 2) - (textH / 2);
            dest.w = textW;
            dest.h = textH;
            SDL_RenderCopy(
                renderer,
                displayText,
                NULL,
                &dest
            );

        }

        SDL_RenderPresent(renderer);
        SDL_Delay(50);

    }

    for (unsigned int i = 0; i < photos.size(); i++) {
        SDL_DestroyTexture(photos[i].texture);
    }
    photos.clear();

    if (loadingIcon) {
        SDL_DestroyTexture(loadingIcon);
    }
    if (displayText) {
        SDL_DestroyTexture(displayText);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 1;
}