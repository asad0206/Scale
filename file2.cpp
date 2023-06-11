#include <iostream>
#include <SDL2/SDL.h>

const int SCREEN_WIDTH = 640, SCREEN_HEIGHT = 480;

// The Window we will be rendering to
SDL_Window *gWindow = NULL;

// Surface contained by the window
SDL_Surface *gScreenSurface = NULL;

// Image we will load and show
SDL_Surface *gImage = NULL;

// Start up sdl and create a window
bool init()
{
    bool success = true;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL could not be initlized! SDL_Error: " << SDL_GetError() << "\n";
        success = false;
    }
    else
    {
        // Create window
        gWindow = SDL_CreateWindow("SDL WINDOW", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

        if (gWindow == NULL)
        {
            std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << "\n";
            success = false;
        }
        else
        {
            // Get window surface
            gScreenSurface = SDL_GetWindowSurface(gWindow);
        }
    }

    return success;
}

// Loads the image
bool loadMedia()
{
    bool success = true;

    // Load splash image
    gImage = SDL_LoadBMP("images/hello_world.bmp");

    if (gImage == NULL)
    {
        std::cout << "Unable to load image! SDL_Error: " << SDL_GetError() << "\n";
        success = false;
    }

    return success;
}

// Frees media and shuts down SDL
void close()
{
    // Deallocate surface
    SDL_FreeSurface(gImage);
    gImage = NULL;

    // Destroy window
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    // Quit sdl subsystems
    SDL_Quit();
}

int main(int argc, char *args[])
{
    // Start up sdl and create window
    if (!init())
    {
        std::cout << "Failed to initialize\n";
    }
    else
    {
        // Load media
        if (!loadMedia())
        {
            std::cout << "Failed to load media\n";
        }
        else
        {
            // Apply image
            SDL_BlitSurface(gImage, NULL, gScreenSurface, NULL);

            // Update the surface
            SDL_UpdateWindowSurface(gWindow);

            // Hack window to stay up
            SDL_Event e;
            bool quit = false;
            while (quit == false)
            {
                while (SDL_PollEvent(&e))
                {
                    if (e.type == SDL_QUIT)
                        quit = true;
                }
            }
        }
    }

    close();

    return EXIT_SUCCESS;
}