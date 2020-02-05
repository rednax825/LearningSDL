#include <iostream>
#include <SDL.h>

#include "res_path.h"
#include "cleanup.h"

int main(int, char**)
{
	// Create vars
	SDL_Window* win;
	SDL_Renderer* ren;
	
	// Init SDL and error on failure
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	
	// Create SDL_Window at 100, 100 with width 640 and height 480 
	win = SDL_CreateWindow("Hello World", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
	// Handle window creation errors and clean up
	if(win == nullptr)
	{
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}
	
	// Create the renderer that will draw to the window
	// -1 specifies that we will use the first graphics driver that supports our selected options
	// option SDL_RENDERER_ACCELERATED requests a hardware accelerated renderer
	// option SDL_RENDERER_PRESENTVSYNC requests that VSync be enabled
	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	// Handle renderer creation errors and clean up
	if(ren == nullptr)
	{
        cleanup(win);
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
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

