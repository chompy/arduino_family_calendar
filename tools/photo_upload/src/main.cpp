#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>

#define MENU_SIZE 32

#define PHOTO_W 200
#define PHOTO_H 240
#define PHOTO_PAD 16

int screenWidth = 640;
int screenHeight = 480;

struct PhotoData
{
    SDL_Texture* texture;
    int position;
};
std::vector<PhotoData> photos;

int main(int argc, char **argv) {

    SDL_SetMainReady();

    SDL_Window* window = SDL_CreateWindow(
        "FamCal Photo Tool",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        screenWidth,
        screenHeight,
        SDL_WINDOW_SHOWN |
        SDL_WINDOW_RESIZABLE
    );
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

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
                    break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer, 0, MENU_SIZE, screenWidth - MENU_SIZE, MENU_SIZE);
        SDL_RenderDrawLine(renderer, screenWidth - MENU_SIZE, MENU_SIZE, screenWidth - MENU_SIZE, screenHeight);

        int availableWidth = screenWidth - MENU_SIZE;
        int imagesPerRow = availableWidth / (PHOTO_W + PHOTO_PAD);
        for (uint i = 0; i < photos.size(); i++) {
            if (!photos[i].texture) {
                continue;
            }
            SDL_Rect dest;
            dest.x = PHOTO_PAD + ((i % imagesPerRow) * (PHOTO_W + PHOTO_PAD));

            dest.y = MENU_SIZE + PHOTO_PAD + ((i / imagesPerRow) * (PHOTO_H + PHOTO_PAD));
            dest.w = PHOTO_W;
            dest.h = PHOTO_H;
            SDL_RenderCopy(
                renderer,
                photos[i].texture,
                NULL,
                &dest
            );
        }
        
        SDL_RenderPresent(renderer);
        SDL_Delay(50);

    }

    for (uint i = 0; i < photos.size(); i++) {
        SDL_DestroyTexture(photos[i].texture);
    }
    photos.clear();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}