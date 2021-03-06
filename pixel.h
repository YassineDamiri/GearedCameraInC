#ifndef PIXEL_H_
#define PIXEL_H_

#include <stdlib.h>
#include "SDL/SDL.h"

Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y);
void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);
void put_pixel_black(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);
void update_surface(SDL_Surface* screen, SDL_Surface* image);

#endif