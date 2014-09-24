#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <stdio.h>
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Starts up SDL and creates window
bool init();

// Loads an image into memory
bool loadMedia();

// Frees up memory and quits SDL
void close();

// Loads a specified image onto a surface
SDL_Surface* loadSurface(std::string path);

// The main window we'll be rendering to. Parent to all surfaces.
SDL_Window* gWindow = NULL;

// The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

// Stretched image
SDL_Surface* gStretchSurface = NULL;


SDL_Surface* loadSurface(std::string path) {
    SDL_Surface* optimizedSurface = NULL;

    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), IMG_GetError());
    } else {
        optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, NULL);
        if (optimizedSurface == NULL) {
            printf("Unable to optimize image! %s\n", SDL_GetError());
        }

        SDL_FreeSurface(loadedSurface);
    }

    return optimizedSurface;
}

bool loadMedia() {
    bool success = true;

    gStretchSurface = loadSurface("loaded.png");
    if (gStretchSurface == NULL) {
        printf("Couldn't load stretch.bmp: %s\n", SDL_GetError());
        success = false;
    }

    return success;
}

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
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) & imgFlags )) {
                printf("SDLimage could not initialize! %s\n", IMG_GetError());
                success = false;
            } else {
                gScreenSurface = SDL_GetWindowSurface(gWindow);
            }
        }
    }

    return success;
}


void close() {
    SDL_FreeSurface(gStretchSurface);
    gStretchSurface = NULL;

    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_Quit();
}

int main(int argc, char* args[]) {
    bool quit = false;

    SDL_Event e;

    if (init()) {
        if (loadMedia()) {
            SDL_Rect stretchRect;
            stretchRect.x = 0;
            stretchRect.y = 0;
            stretchRect.w = SCREEN_WIDTH;
            stretchRect.h = SCREEN_HEIGHT;
            SDL_BlitScaled(gStretchSurface, NULL, gScreenSurface, &stretchRect);
            SDL_UpdateWindowSurface(gWindow);
            while(!quit) {
                while(SDL_PollEvent(&e) != 0) {

                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }
                }
            }
        }
    }

    close();

    return 0;
}
