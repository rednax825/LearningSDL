#ifndef CLEANUP_H
#define CLEANUP_H

#include <utility>
#include <SDL.h>

/*
 * Recursive cleanup function, takes template list
 * uses specialized template functions at each iteration
 */
template<typename T, typename... Args>
void cleanup(T* t, Args&&... args)
{
    // call the specialized template function on the first arg
    cleanup(t);
    // recurse to clean the rest of the args
    cleanup(std::forward<Args>(args)...);
}

/*
 * The below specializations clean up each type of SDL resource
 */
template<>
inline void cleanup<SDL_Window>(SDL_Window* win)
{
    if(!win)
    {
        return;
    }
    SDL_DestroyWindow(win);
}

template<>
inline void cleanup<SDL_Renderer>(SDL_Renderer* ren)
{
    if(!ren)
    {
        return;
    }
    SDL_DestroyRenderer(ren);
}

template<>
inline void cleanup<SDL_Texture>(SDL_Texture* tex)
{
    if(!tex)
    {
        return;
    }
    SDL_DestroyTexture(tex);
}

template<>
inline void cleanup<SDL_Surface>(SDL_Surface* surf)
{
    if(!surf)
    {
        return;
    }
    SDL_FreeSurface(surf);
}

#endif
