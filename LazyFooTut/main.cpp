#include <SDL2/SDL.h>
#include <stdio.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* gWindow = NULL;

SDL_Surface* gScreenSurface = NULL;

SDL_Surface* gHelloWorld = NULL;

bool init() {
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Failed to init %s\n", SDL_GetError());
        success = false;
    } else {
        gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow  == NULL) {
            printf("Failed to create window: %s\n", SDL_GetError());
            success = false;
        }  else {
            gScreenSurface = SDL_GetWindowSurface(gWindow);
        }
    }

    return success;
}


bool loadMedia() {
    bool success = true;

    gHelloWorld = SDL_LoadBMP("marbles.bmp");
    if (gHelloWorld == NULL) {
        printf("Could not load media: %s\n", SDL_GetError());
        success = false;
    }

    return success;
}

void close() {
    SDL_FreeSurface(gHelloWorld);
    gHelloWorld = NULL;

    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_Quit();
}

int main(int argc, char* args[]) {
    if (init() >= 0) {
        if (loadMedia() >= 0) {
            SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);
            SDL_UpdateWindowSurface(gWindow);
            SDL_Delay(2000);
        } else {
            printf("Could not load media");
        }
    } else {
        printf("Could not init");
    }

    close();

    return 0;
}
