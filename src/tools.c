#include <err.h>

#include "tools.h"

    static inline
Uint8* pixel_ref(SDL_Surface *surf, unsigned x, unsigned y)
{
    int bpp = surf->format->BytesPerPixel;
    return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}

Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y)
{
    Uint8 *p = pixel_ref(surface, x, y);

    switch (surface->format->BytesPerPixel)
    {
        case 1:
            return *p;

        case 2:
            return *(Uint16 *)p;

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;

        case 4:
            return *(Uint32 *)p;
    }

    return 0;
}

Uint8 get_v(SDL_Surface *surface, unsigned x, unsigned y)
{
    Uint8 r, g, b;
    SDL_GetRGB(get_pixel(surface, x, y), surface->format, &r, &g, &b);
    return (r + g + b)/3;

}

Uint8* get_rgb(SDL_Surface *surface, unsigned x, unsigned y)
{
    static Uint8 a[3];
    SDL_GetRGB(get_pixel(surface, x, y), surface->format, &a[0], &a[1], &a[2]);
    return a;
}

void getRGB(SDL_Surface *surface, unsigned x, unsigned y, Uint8* r,
        Uint8* g, Uint8* b)
{
    SDL_GetRGB(get_pixel(surface, x, y), surface->format, r, g ,b);
    return ;
}

void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel)
{
    Uint8 *p = pixel_ref(surface, x, y);

    switch(surface->format->BytesPerPixel)
    {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(Uint16 *)p = pixel;
            break;

        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            }
            else
            {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32 *)p = pixel;
            break;
    }
}


SDL_Surface* new_rgb_surface(int width, int height)
{
    SDL_Surface *newSurface;

    Uint32 rmask, gmask, bmask, amask;
    if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
    {
        rmask = 0xff000000;
        gmask = 0x00ff0000;
        bmask = 0x0000ff00;
        amask = 0x000000ff;
    }
    else
    {
        rmask = 0x000000ff;
        gmask = 0x0000ff00;
        bmask = 0x00ff0000;
        amask = 0xff000000;
    }

    newSurface =
        SDL_CreateRGBSurface(0, width, height, 32, rmask, gmask, bmask, amask);

    if (newSurface == NULL)
    {
        printf("SDL_CreateRGBSurface() failed: %s\n", SDL_GetError());
        exit(1);
    }

    return newSurface;
}

void update_surface(SDL_Surface* screen, SDL_Surface* image)
{
    if (SDL_BlitSurface(image, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    SDL_UpdateRect(screen, 0, 0, image->w, image->h);
}
