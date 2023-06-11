#include <iostream>
#include <SDL2/SDL.h>

const int SCREEN_WIDTH = 640, SCREEN_HEIGHT = 480;

// The Window we will be rendering to
SDL_Window *gWindow = NULL;

// Surface contained by the window
SDL_Surface *gScreenSurface = NULL;

// Image we will load and show
SDL_Surface *gXOut = NULL;

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
        gWindow = SDL_CreateWindow("SDL WINDOW", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

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
    gXOut = SDL_LoadBMP("images/x.bmp");

    if (gXOut == NULL)
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
    SDL_FreeSurface(gXOut);
    gXOut = NULL;

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
            // Main loop flag
            bool quit = false;

            // Event handler
            SDL_Event e;

            // While application is running
            while (!quit)
            {
                // Handle events on queue
                while (SDL_PollEvent(&e) != 0)
                {
                    // User requests quit
                    if (e.type == SDL_QUIT)
                        quit = true;
                }

                // Apply image
                SDL_BlitSurface(gXOut, NULL, gScreenSurface, NULL);

                // Update the surface
                SDL_UpdateWindowSurface(gWindow);
            }
        }
    }

    close();

    return EXIT_SUCCESS;
}