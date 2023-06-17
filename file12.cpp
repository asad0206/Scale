/*Color modulation*/

#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

const int SCREEN_WIDTH = 640, SCREEN_HEIGHT = 480;

class LTexture
{
public:
    // Initializes variables
    LTexture();

    // Deallocate the mem
    ~LTexture();

    // Load image at specified path
    bool loadFromFile(std::string path);

    // Deallocates textures
    void free();

    // Set color modulation
    void setColor(Uint8 red, Uint8 green, Uint8 blue);

    // Render the texture
    void render(int x, int y, SDL_Rect *clip = NULL);

    // Fetch image dimensions
    int getWidth();
    int getHeight();

private:
    // The actual hardware texture
    SDL_Texture *mTexture;

    // Image dimensions
    int mWidth;
    int mHeight;
};

// Starts up SDL and creates window
bool init();

// Loads media
bool loadMedia();

// Frees media and shuts down SDL
void close();

// The window we'll be rendering to
SDL_Window *gWindow = NULL;

// The window renderer
SDL_Renderer *gRenderer = NULL;

// Scene texture
LTexture gModulatedTexture;

LTexture::LTexture()
{
    // Intialize
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture()
{
    // Deallocate
    free();
}

bool LTexture::loadFromFile(std::string path)
{
    // Getting rid of preexisiting textures
    free();

    // Final texure
    SDL_Texture *newTexture = NULL;

    // Load image
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        std::cout << "Unable to load image " << path.c_str() << " SDL_image Error: " << IMG_GetError() << "\n";
    }
    else
    {
        // Color key image
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

        // Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);

        if (newTexture == NULL)
        {
            std::cout << "Unable to create texture from " << path.c_str() << " SDL Error: " << SDL_GetError() << "\n";
        }
        else
        {
            // Get image dimensions
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }

        // Getting rid of the old surface
        SDL_FreeSurface(loadedSurface);
    }

    // Return success
    mTexture = newTexture;
    return mTexture != NULL;
}

void LTexture::free()
{
    // Free texture if it exists
    if (mTexture != NULL)
    {
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
    // Modulate texture
    SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::render(int x, int y, SDL_Rect *clip)
{
    // Set the rendering space and render to the screen
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};

    // Set the clip rendering dimensions
    if (clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    // Render the texture to screen
    SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
}

int LTexture::getWidth()
{
    return mWidth;
}

int LTexture::getHeight()
{
    return mHeight;
}

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
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

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
    // Loading success flag
    bool success = true;

    // Load texture
    if (!gModulatedTexture.loadFromFile("images/colors.png"))
    {
        printf("Failed to load colors texture!\n");
        success = false;
    }

    return success;
}

void close()
{
    // Free loaded images
    gModulatedTexture.free();

    // Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    // Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
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

            // Modulation components
            Uint8 r = 255;
            Uint8 g = 255;
            Uint8 b = 255;

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

                    else if (e.type == SDL_KEYDOWN)
                    {
                        switch (e.key.keysym.sym)
                        {
                        // Increse red
                        case SDLK_q:
                            r += 32;
                            break;

                        // Increase green
                        case SDLK_w:
                            g += 32;
                            break;

                        // Increase blue
                        case SDLK_e:
                            b += 32;
                            break;

                        // Decrease red
                        case SDLK_a:
                            r -= 32;
                            break;

                        // Decrease green
                        case SDLK_s:
                            g -= 32;
                            break;

                        // Decrease blue
                        case SDLK_d:
                            b -= 32;
                            break;
                        }
                    }
                }

                // Clear the screen
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);

                // Modulate and render
                gModulatedTexture.setColor(r, g, b);
                gModulatedTexture.render(0, 0);

                // Update screen
                SDL_RenderPresent(gRenderer);
            }
        }
    }

    // Free resources
    close();

    return EXIT_SUCCESS;
}