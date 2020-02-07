#include <iostream>
#include <SDL.h>

#include "res_path.h"
#include "cleanup.h"

// Declare constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Function Prototypes
void logSDLError(std::ostream& os, const std::string& msg);
SDL_Texture* loadTexture(const std::string& file, SDL_Renderer* ren);
void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y);

int main(int, char**)
{
	// Create vars
	SDL_Window* window;
	SDL_Renderer* renderer;
	
	// Init SDL and error on failure
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
        logSDLError(std::cout, "SDL_Init");
		return 1;
	}
	
	// Create SDL_Window at 100, 100
	window = SDL_CreateWindow("LearningSDL", 100, 100, SCREEN_WIDTH,
            SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	// Handle window creation errors and clean up
	if(window == nullptr)
	{
        logSDLError(std::cout, "CreateWindow");
		SDL_Quit();
		return 1;
	}
	
	// Create the renderer that will draw to the window
	// -1 specifies that we will use the first graphics driver that supports our selected options
	// option SDL_RENDERER_ACCELERATED requests a hardware accelerated renderer
	// option SDL_RENDERER_PRESENTVSYNC requests that VSync be enabled
	renderer = SDL_CreateRenderer(window, -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	// Handle renderer creation errors and clean up
	if(renderer == nullptr)
	{
        logSDLError(std::cout, "CreateRenderer");
        cleanup(window);
		SDL_Quit();
		return 1;
	}
	
	// Load the image and create and SDL_Surface
	// Surfaces are image data in standard memory
	// Surfaces can be used to create textures, which are image data in GPU memory
	std::string imagePath = getResourcePath() + "hello.bmp";
	SDL_Surface *bmp = SDL_LoadBMP(imagePath.c_str());
	// Handle SDL_Surface creation errors and clean up
	if(bmp == nullptr)
	{
        cleanup(ren, win);
		std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}
	
	// Create a texture from the surface
	SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, bmp);
	// Release the surface from memory
    cleanup(bmp);
	// Handle texture creation errors and clean up
	if(tex == nullptr)
	{
        cleanup(ren, win);
		std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}
	
	// Sleepy rendering loop to draw the texture
	for(int i = 0; i < 3; i++)
	{
		// Clear the renderer
		SDL_RenderClear(ren);
		// Draw the texture
		SDL_RenderCopy(ren, tex, NULL, NULL);
		// Update the screen
		SDL_RenderPresent(ren);
		// Sleep one second
		SDL_Delay(1000);
	}
	
	// Clean up objects and quit
    cleanup(tex, ren, win);
	SDL_Quit();

	return 0;
}

/**
 * logSDLError: this function logs and SDL error message to the given output stream
 * @param os The output stream to which the error is written to
 * @param msg the content of the error message
 */
void logSDLError(std::ostream& os, const std::string& msg)
{
    os << msg << " error: " << SDL_GetError() << std::endl;
}

/**
 * loadTexture: thus function loads a BMP image into a texture on the rendering device
 * @param file The BMP file to load
 * @param ren The renderer to load the texture to
 * @return the loaded texture, or nullptr if something went wrong
 */
SDL_Texture* loadTexture(const std::string& file, SDL_Renderer* ren)
{
    // Init to nullptr to avoid dangling pointer issues
    SDL_Texture* texture = nullptr;
    // Load the image
    SDL_Surface* loadedImage = SDL_LoadBMP(file.c_str());
    // If the image was loaded successfully, convert and return the texture
    if(loadedImage != nullptr)
    {
        texture = SDL_CreateTextureFromSurface(ren, loadedImage);
        SDL_FreeSurface(loadedImage);
        // Make sure the conversion was successful
        if(texture == nullptr)
        {
            logSDLError(std::cout, "CreateTextureFromSurface");
        }
    }
    else
    {
        logSDLError(std::cout, "LoadBMP");
    }
    return texture;
}

/**
 * renderTexture: Draw an SDL_Texture to an SDL_Renderer at position x, y,
 * preserving the texture's width and height
 * @param tex The source texture
 * @param ren The renderer to which the texture will be drawn
 * @param x The x coordinate to draw to
 * @param y The y coordinate to draw to
 */
void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y)
{
    // Setup the destination rectangle to be at position x, y
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    // Query the texture to get its width and height
    SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy(ren, tex, NULL, &dst);
}

