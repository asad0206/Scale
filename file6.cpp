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
SDL_Surface *loadSurface(std::string path);

// The window we'll be rendering to
SDL_Window *gWindow = NULL;

// The surface contained by the window
SDL_Surface *gScreenSurface = NULL;

// Current displayed image
SDL_Surface *gPNGSurface = NULL;

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
        // Create window
        gWindow = SDL_CreateWindow("SDL WINDOW", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

        if (gWindow == NULL)
        {
            std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << "\n";
            success = false;
        }
        else
        {
            // Initialize the PNG loadingg
            int imgFlags = IMG_INIT_PNG;

            if (!(IMG_Init(imgFlags) & imgFlags))
            {
                std::cout << "SDL_image could not be initialized! SDL_imgae error: " << IMG_GetError() << "\n";
                success = false;
            }

            else
            {
                // Get window surface
                gScreenSurface = SDL_GetWindowSurface(gWindow);
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
    gPNGSurface = loadSurface("images/loaded.png");
    if (gPNGSurface == NULL)
    {
        std::cout << "Failed to load PNG!\n";
        success = false;
    }

    return success;
}

void close()
{
    // Free up loaded image
    SDL_FreeSurface(gPNGSurface);
    gPNGSurface = NULL;

    // Destroy window
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    // Quit sdl subsystems
    SDL_Quit();
}

SDL_Surface *loadSurface(std::string path)
{
    // Final optimized image
    SDL_Surface *optimizedSurface = NULL;

    // Load image at the specified path
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        std::cout << "Unable to load the image " << path.c_str() << " SDL_image Error: " << IMG_GetError() << "\n";
    }
    else
    {
        // Convert surface to the screen format
        optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
        if (optimizedSurface == NULL)
        {
            std::cout << "Unable to optimize image " << path.c_str() << " SDL_image Error: " << SDL_GetError() << "\n";
        }

        // Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }
    return optimizedSurface;
}

int main(int argc, char *args[])
{
    // Start up sdl and creating the window
    if (!init())
    {
        std::cout << "Failed to initialize!\n";
    }
    else
    {
        // Load Media
        if (!loadMedia())
        {
            std::cout << "Failed to load media!\n";
        }
        else
        {
            // Main loop flag
            bool quit = false;

            // Event handler
            SDL_Event e;

            // When the application running
            while (!quit)
            {
                // Handle events on queue
                while (SDL_PollEvent(&e) != 0)
                {
                    // User requests quit
                    if (e.type == SDL_QUIT)
                        quit = true;
                }

                // Apply the PNG image
                SDL_BlitSurface(gPNGSurface, NULL, gScreenSurface, NULL);

                // Update the srface
                SDL_UpdateWindowSurface(gWindow);
            }
        }
    }

    // Free up resources and close sdl
    close();

    return EXIT_SUCCESS;
}