#include <SDL2/SDL.h>
#include <SDL2/SDL_hints.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define WWIDTH 1024
#define WHEIGHT 768 
#define TWIDTH 512
#define THEIGHT 512
#define BUFLEN TWIDTH*THEIGHT*sizeof(int)
int buffer[BUFLEN];

int main(void) {
    SDL_SetHint("SDL_RENDER_DRIVER", "opengl");
    SDL_Window *window = SDL_CreateWindow("Prova", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WWIDTH, WHEIGHT, SDL_WINDOW_OPENGL);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WWIDTH, WHEIGHT);
    SDL_Surface *surface = IMG_LoadBMP_RW(SDL_RWFromFile("assets/backrooms.bmp", "r"));
    SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_ARGB8888, 0);
    void *pixel;
    int pitch;
    // SDL_LockTexture(texture, NULL, &pixel, &pitch);
    // SDL_UnlockTexture(texture);
    SDL_LockSurface(surface);
    SDL_LockTexture(texture, NULL, &pixel, &pitch);
    
    Uint8 *pixels = (Uint8*)surface->pixels;
    // pixels[200*surface->w*surface->format->BytesPerPixel]=0xFF;
    for (int i = 0; i < surface->h; i++) {
        for (int j = 0; j<surface->w; j++) {
            if (j == 404) {
                printf("ciao");
            }
            Uint8 b = pixels[i*surface->w*surface->format->BytesPerPixel+j*surface->format->BytesPerPixel]; 
            Uint8 g = pixels[i*surface->w*surface->format->BytesPerPixel+j*surface->format->BytesPerPixel+1];
            Uint8 r = pixels[i*surface->w*surface->format->BytesPerPixel+j*surface->format->BytesPerPixel+2];
            Uint8 a = pixels[i*surface->w*surface->format->BytesPerPixel+j*surface->format->BytesPerPixel+3]; 
            buffer[i*WWIDTH+j]  = pixels[i*surface->w*surface->format->BytesPerPixel+j*surface->format->BytesPerPixel]; 
            buffer[i*WWIDTH+j] |= pixels[i*surface->w*surface->format->BytesPerPixel+j*surface->format->BytesPerPixel+1]<<8; 
            buffer[i*WWIDTH+j] |= pixels[i*surface->w*surface->format->BytesPerPixel+j*surface->format->BytesPerPixel+2]<<16; 
            buffer[i*WWIDTH+j] |= pixels[i*surface->w*surface->format->BytesPerPixel+j*surface->format->BytesPerPixel+3]<<24; 
        }
    }
    printf("w: %d - h: %d", surface->w, surface->h);
    memcpy(pixel, buffer, BUFLEN);
    SDL_UnlockTexture(texture);
    SDL_UnlockSurface(surface);
    // texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_Delay(4000);
}