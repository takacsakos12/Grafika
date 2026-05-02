#include "texture.h"

#include <SDL2/SDL.h>
#include <stdio.h>

GLuint load_texture(const char* filename)
{
    SDL_Surface* surface;
    SDL_Surface* formatted_surface;
    GLuint texture_id;

    surface = SDL_LoadBMP(filename);

    if (surface == NULL) {
        fprintf(stderr, "Texture loading error: %s - %s\n", filename, SDL_GetError());
        return 0;
    }

    formatted_surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);

    if (formatted_surface == NULL) {
        fprintf(stderr, "Texture format conversion error: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return 0;
    }

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        formatted_surface->w,
        formatted_surface->h,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        formatted_surface->pixels
    );

    printf("Texture loaded: %s, id: %u, size: %dx%d\n",
        filename,
        texture_id,
        formatted_surface->w,
        formatted_surface->h
    );

    SDL_FreeSurface(formatted_surface);
    SDL_FreeSurface(surface);

    return texture_id;
}