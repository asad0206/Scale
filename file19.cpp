/*Sound effects*/

#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

// Screen dimensions
const int SCREEN_WIDTH = 640, SCREEN_HEIGHT = 480;

// Texture wrapper class
class LTexture
{
public:
    // Initializes variables
    LTexture();

    // Deallocates memory
    ~LTexture();

    // Loads image at specified path
    bool loadFromFile(std::string path);

#if defined(SDL_TTF_MAJOR_VERSION)
    // Creates image from font string
    bool loadFromRenderedText(std::string textureText, SDL_Color textColor);
#endif

    // Deallocates texture
    void free();

    // Set color modulation
    void setColor(Uint8 red, Uint8 green, Uint8 blue);

    // Set blending
    void setBlendMode(SDL_BlendMode blending);

    // Set alpha modulation
    void setAlpha(Uint8 alpha);

    // Renders texture at given point
    void render(int x, int y, SDL_Rect *clip = NULL, double angle = 0.0, SDL_Point *center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

    // Gets image dimensions
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

// Scene textures
LTexture gPromptTexture;

// Music that will be played
Mix_Music *gMusic = NULL;

// The sound effect that will be used
Mix_Chunk *gScratch = NULL;
Mix_Chunk *gHigh = NULL;
Mix_Chunk *gMedium = NULL;
Mix_Chunk *gLow = NULL;

LTexture::LTexture()
{
    // Initialize
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

#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor)
{
    // Get rid of preexisting texture
    free();

    // Render text surface
    SDL_Surface *textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
    if (textSurface != NULL)
    {
        // Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        if (mTexture == NULL)
        {
            printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        }
        else
        {
            // Get image dimensions
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }

        // Get rid of old surface
        SDL_FreeSurface(textSurface);
    }
    else
    {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    }

    // Return success
    return mTexture != NULL;
}
#endif

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
    // Modulate texture rgb
    SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
    // Set blending function
    SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha)
{
    // Modulate texture alpha
    SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(int x, int y, SDL_Rect *clip, double angle, SDL_Point *center, SDL_RendererFlip flip)
{
    // Set rendering space and render to screen
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};

    // Set clip rendering dimensions
    if (clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    // Render to screen
    SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
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
    // Initialization flag
    bool success = true;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        // Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            std::cout << "Warning: Linear texture filtering not enabled!\n";
        }

        // Create window
        gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            std::cout << "Window could not be created! SDL Error: " << SDL_GetError() << "\n";
            success = false;
        }
        else
        {
            // Create vsynced renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (gRenderer == NULL)
            {
                std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << "\n";
                success = false;
            }
            else
            {
                // Initialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                // Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << "\n";
                    success = false;
                }

                // Initialize SDL_mixer
                if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
                {
                    std::cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << "\n";
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

    // Load press texture
    if (!gPromptTexture.loadFromFile("images/prompt.png"))
    {
        std::cout << "Failed to load PROMPT texture!\n";
        success = false;
    }

    // Load music
    gMusic = Mix_LoadMUS("music/beat.wav");
    if (gMusic == NULL)
    {
        std::cout << "Failed to load BEAT music! SDL_mixer error: " << Mix_GetError() << "\n";
        success = false;
    }

    // Load sound effects
    gScratch = Mix_LoadWAV("music/scratch.wav");
    if (gScratch == NULL)
    {
        std::cout << "Failed to load SCRATCH sound effect! SDL_mixer error: " << Mix_GetError() << "\n";
        success = false;
    }

    gHigh = Mix_LoadWAV("music/high.wav");
    if (gHigh == NULL)
    {
        std::cout << "Failed to load HIGH sound effect! SDL_mixer error: " << Mix_GetError() << "\n";
        success = false;
    }

    gMedium = Mix_LoadWAV("music/medium.wav");
    if (gMedium == NULL)
    {
        std::cout << "Failed to load MEDIUM sound effect! SDL_mixer error: " << Mix_GetError() << "\n";
        success = false;
    }

    gLow = Mix_LoadWAV("music/low.wav");
    if (gLow == NULL)
    {
        std::cout << "Failed to load LOW sound effect! SDL_mixer error: " << Mix_GetError() << "\n";
        success = false;
    }

    return success;
}

void close()
{
    // Free loaded images
    gPromptTexture.free();

    // Free Music
    Mix_FreeMusic(gMusic);
    gMusic = NULL;

    // Free sound
    Mix_FreeChunk(gScratch);
    Mix_FreeChunk(gHigh);
    Mix_FreeChunk(gMedium);
    Mix_FreeChunk(gLow);

    gScratch = NULL;
    gHigh = NULL;
    gMedium = NULL;
    gLow = NULL;

    // Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gRenderer = NULL;
    gWindow = NULL;

    // Quit SDL subsytems
    Mix_Quit();
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
                    {
                        quit = true;
                    }

                    // Handle key presses
                    else if (e.type == SDL_KEYDOWN)
                    {
                        switch ((e.key.keysym.sym))
                        {
                        // Play high sound effect
                        case SDLK_1:
                            Mix_PlayChannel(-1, gHigh, 0);
                            break;

                        // Play medium sound effect
                        case SDLK_2:
                            Mix_PlayChannel(-1, gMedium, 0);
                            break;

                        // Play low sound effect
                        case SDLK_3:
                            Mix_PlayChannel(-1, gLow, 0);
                            break;

                        // Play scratch sound effect
                        case SDLK_4:
                            Mix_PlayChannel(-1, gScratch, 0);
                            break;

                        case SDLK_9:
                            // If no music plays
                            if (Mix_PlayingMusic() == 0)
                            {
                                // Play music
                                Mix_PlayMusic(gMusic, -1);
                            }

                            // If Music is played
                            else
                            {
                                // If music is paused
                                if (Mix_PausedMusic() == 1)
                                {
                                    // Resume music
                                    Mix_ResumeMusic();
                                }
                                else
                                {
                                    // Pause music
                                    Mix_PauseMusic();
                                }
                            }
                            break;

                        case SDLK_0:
                            // Stop the music
                            Mix_HaltMusic();
                            break;
                        }
                    }
                }

                // Clear screen
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);

                // Render prompt texture
                gPromptTexture.render(0, 0);

                // Update screen
                SDL_RenderPresent(gRenderer);
            }
        }
    }

    // Free resources
    close();

    return EXIT_SUCCESS;
}
