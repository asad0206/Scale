/*Texture rendering*/

#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

const int SCREEN_WIDTH = 640, SCREEN_HEIGHT = 480;

// Starts up SDL and creates window
bool init();

// Loads media
bool loadMedia();

// Frees media and shuts down SDL
void close();

// Loads individual image
SDL_Texture *loadTexture(std::string path);

// The window we'll be rendering to
SDL_Window *gWindow = NULL;

// The window renderer
SDL_Renderer *gRenderer = NULL;

// Current displayed image
SDL_Texture *gTexture = NULL;

bool init()
{
    // Flag
    bool success = true;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL could not be initlized! SDL_Error: " << SDL_GetError() << "\n";
        success = false;
    }
    else
    {
        // Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            std::cout << "Warning: Linear texturing filtering not enabled!\n";
        }

        // Create window
        gWindow = SDL_CreateWindow("SDL Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

        if (gWindow == NULL)
        {
            std::cout << "Window could not be created! SDL Error: " << SDL_GetError();
            success = false;
        }
        else
        {
            // Create renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

            if (gRenderer == NULL)
            {
                std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << "\n";
                success = false;
            }
            else
            {
                // Intialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff);

                // Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << "\n";
                    success = false;
                }
            }
        }
    }

    return success;
}

bool loadMedia()
{
    // Flag
    bool success = true;

    // Load png surface
    gTexture = loadTexture("images/texture.png");
    if (gTexture == NULL)
    {
        std::cout << "Failed to load texture image!\n";
        success = false;
    }

    return success;
}

void close()
{
    // Free up loaded image
    SDL_DestroyTexture(gTexture);
    gTexture = NULL;

    // Destroy window and Renderer
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gRenderer = NULL;
    gWindow = NULL;

    // Quit sdl subsystems
    IMG_Quit();
    SDL_Quit();
}

SDL_Texture *loadTexture(std::string path)
{
    // The final texture
    SDL_Texture *newTexture = NULL;

    // Load image at specified path
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        std::cout << "Unable to load image " << path.c_str() << " SDL_image Error: " << IMG_GetError() << "\n";
    }
    else
    {
        // Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);

        if (newTexture == NULL)
        {
            std::cout << "Unable to create texture from " << path.c_str() << " SDL Error: " << SDL_GetError() << "\n";
        }

        // Get rid of the old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}

int main(int argc, char *args[])
{
    // Initialize sdl
    if (!init())
    {
        std::cout << "Failed to initialize!\n";
    }
    else
    {
        // Load media
        if (!loadMedia())
        {
            std::cout << "Failed to load the media!\n";
        }
        else
        {
            // Flag
            bool quit = false;

            // Event handler
            SDL_Event e;

            // While the application runs
            while (!quit)
            {
                while (SDL_PollEvent(&e) != 0)
                {
                    // User requests quit
                    if (e.type == SDL_QUIT)
                    {
                        quit = true;
                    }
                }

                // Clear the screen
                SDL_RenderClear(gRenderer);

                // Render texture to the screen
                SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

                // Update screen
                SDL_RenderPresent(gRenderer);
            }
        }
    }

    // Free resources
    close();

    return EXIT_SUCCESS;
}