#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include "SerialClass.h"

#define MENU_SIZE 32

#define PHOTO_W 200
#define PHOTO_H 240
#define PHOTO_PAD 16

#define SP_STATE_NONE 0
#define SP_STATE_WAIT 1
#define SP_STATE_PHOTO_COUNT 2
#define SP_STATE_PHOTO_REQUEST 3
#define SP_STATE_PHOTO_DOWNLOAD 4

int screenWidth = 640;
int screenHeight = 480;

struct PhotoData
{
    SDL_Texture* texture;
    int position;
};
std::vector<PhotoData> photos;

int viewOffset = 0;

int spState = 0;
unsigned char devicePhotoCount = 0;
unsigned char devicePhotoNum = 0;
std::vector<char> photoData;

SDL_Renderer* renderer = nullptr;
SDL_Window* window = nullptr;
SDL_Texture* loadingIcon = nullptr;

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

    Serial* SP = new Serial("\\\\.\\COM7");

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

    // load assets
    SDL_Surface* loadingIconSurface = IMG_Load("assets/loading.png");
    if (loadingIconSurface) {
        loadingIcon = SDL_CreateTextureFromSurface(renderer, loadingIconSurface);
        SDL_FreeSurface(loadingIconSurface);
    }

    SDL_SetMainReady();

    drawLoading();

    SDL_Event e;
    bool loop = true;
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
            }
        }

        if (!SP->IsConnected()) {
            SDL_Delay(50);
            continue;
        }

        switch (spState) {
            case SP_STATE_NONE:
            {
                std::cout << "* Connected! Requesting photo count." << std::endl;
                spState = SP_STATE_PHOTO_COUNT;
                char state = SP_STATE_PHOTO_COUNT;
                SP->WriteData(&state, 1);
                break;
            }
            case SP_STATE_PHOTO_COUNT:
            {
                char buf[1];
                int results = SP->ReadData(buf, 1);
                if (results > 0) {
                    devicePhotoCount = (unsigned char) buf[0];
                    std::cout << "* Photo count is " << (int) devicePhotoCount << "." << std::endl;
                    for (unsigned char i = 0; i < devicePhotoCount; i++) {
                        PhotoData pd;
                        pd.position = 0;
                        pd.texture = nullptr;
                        photos.push_back(pd);
                    }
                    spState = SP_STATE_PHOTO_DOWNLOAD;

                    std::cout << "* Request photo #0." << std::endl;
                    char photoRequest[2];
                    photoRequest[0] = SP_STATE_PHOTO_REQUEST;
                    photoRequest[1] = 0;
                    SP->WriteData(photoRequest, 2);
                    devicePhotoNum = 0;
                    photoData.clear();
                    std::cout << "TEST??" << std::endl;
                    
                }
                break;
            }
            case SP_STATE_PHOTO_DOWNLOAD:
            {
                char buf[255];
                int results = SP->ReadData(buf, 255);
                if (results > 0) {
                    std::cout << "GOT SOMETHING" << std::endl;
                    unsigned short width, height;
                    memcpy(&width, &buf[0], 2);
                    memcpy(&height, &buf[2], 2);
                    std::cout << "Recieving photo data #" << (int) devicePhotoNum << " ("  << width << "x" << height << "px)." << std::endl;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer, 0, MENU_SIZE, screenWidth - MENU_SIZE, MENU_SIZE);
        SDL_RenderDrawLine(renderer, screenWidth - MENU_SIZE, MENU_SIZE, screenWidth - MENU_SIZE, screenHeight);

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
        
        SDL_RenderPresent(renderer);
        SDL_Delay(50);

    }

    for (unsigned int i = 0; i < photos.size(); i++) {
        SDL_DestroyTexture(photos[i].texture);
    }
    photos.clear();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
}