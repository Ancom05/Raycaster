#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_video.h>
#include <math.h>
#include "linear_alg.h"

#define PI 3.14159f
#define WWIDTH 1024
#define WHEIGHT 768
#define MWIDTH 24
#define MHEIGHT 24

int map[MWIDTH][MHEIGHT]=
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};


typedef struct Player {
    Vector2 position;
    Vector2 direction;
    Vector2 screen;
} Player;

void castRays(SDL_Renderer *renderer, Player *player, int mapScaler);

// typedef struct ScaledProjection {
//     int width;
//     int height;
//     int scale;
// } ScaledProjection;

int main(void) {
        if (SDL_Init(SDL_INIT_EVERYTHING)==-1) {
        printf("Couldn't initialize SDL");
    }
    int scale = 1;
    int mapScaler = 32;
    // ScaledProjection scaledProjection = {WWIDTH/scale, WHEIGHT/scale, scale};
    int fps = 60;   
    SDL_Event keyboardEvent;
    Uint32 delayMs = (double)(((double)1.0f / (double)fps))*645;
    const Uint8 *pressedKeys = SDL_GetKeyboardState(NULL); 
    // Player player = {WWIDTH/2, WHEIGHT/2, 90, 0};
    Vector2 initialPos = {WWIDTH/2, WHEIGHT/2};
    Vector2 initialDir = {0, -1*mapScaler};
    Vector2 screen = {1*mapScaler, 0}; //perpendicolare alla direzione e di stessa lunghezza, così da avere fov=90°
    Player player = {initialPos, initialDir, screen};
    SDL_Window *window;
    SDL_Renderer *renderer;
    // uint8_t **map = createRectMap(mapSize, mapSize);
    window =  SDL_CreateWindow("Dumb Raycaster", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WWIDTH, WHEIGHT, SDL_WINDOW_OPENGL);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    // initColors();
    SDL_RenderSetScale(renderer, scale, scale);
    int isGameRunning = 1;
    while (isGameRunning) {
        Uint32 lastTick = SDL_GetTicks();
        SDL_RenderClear(renderer);
        while (SDL_PollEvent(&keyboardEvent)) { //da spostare in una funzione a parte
            if (keyboardEvent.type == SDL_QUIT) {
                isGameRunning = 0;
            }
        }
        if (pressedKeys[SDL_SCANCODE_LEFT]) {
            rotateVector(&player.direction, -0.014); //circa 1°
            rotateVector(&player.screen, -0.014);
        } 
        if (pressedKeys[SDL_SCANCODE_RIGHT]) {
            rotateVector(&player.direction, 0.014); //circa 1°
            rotateVector(&player.screen, 0.014);
        }    
        if (pressedKeys[SDL_SCANCODE_UP]) {
            player.position.x+=player.direction.x/mapScaler;
            player.position.y+=player.direction.y/mapScaler;
        } 
        if (pressedKeys[SDL_SCANCODE_DOWN]) {
            player.position.x-=player.direction.x/mapScaler;
            player.position.y-=player.direction.y/mapScaler;
        }   
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (int i = 0; i < MHEIGHT; i++) {
            for (int j = 0; j < MWIDTH; j++) {
                if (map[i][j]!=0) {
                    SDL_Rect rect = {j*mapScaler, i*mapScaler, mapScaler, mapScaler};
                    SDL_RenderDrawRect(renderer, &rect);
                }
            }
        }
        //INIZIO render 2D di prova
        SDL_RenderDrawPointF(renderer, player.position.x, player.position.y);
        SDL_RenderDrawLineF(renderer, player.position.x, player.position.y, player.position.x + player.direction.x , player.position.y + player.direction.y );
        SDL_RenderDrawLineF(renderer, player.position.x+player.direction.x, player.position.y + player.direction.y , player.position.x+player.direction.x+player.screen.x, player.position.y+player.direction.y+player.screen.y);
        SDL_RenderDrawLineF(renderer, player.position.x+player.direction.x, player.position.y + player.direction.y , player.position.x+player.direction.x-player.screen.x, player.position.y+player.direction.y-player.screen.y);
        //FINE render 2D di prova
        castRays(renderer, &player, mapScaler);
        SDL_RenderPresent(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        Uint32 currentDelay = SDL_TICKS_PASSED(lastTick, SDL_GetTicks());
        if (currentDelay<0) currentDelay = 0;
        SDL_Delay(delayMs - currentDelay);
    }
}

void castRays(SDL_Renderer *renderer, Player *player, int mapScaler) {
    for (int i = 0; i < WWIDTH; i++) {
        double cameraX = (float)2 * i / WWIDTH - 1;
        Vector2 ray;
        ray.x = player->direction.x + player->screen.x * cameraX;
        ray.y = player->direction.y + player->screen.y * cameraX;
        Vector2 tmp;
        tmp.x = ray.x + player->position.x;
        tmp.y = ray.y + player->position.y;
        Vector2 unaryLength;
        if (ray.x != 0) { // perchè non puoi dividere per 0
            unaryLength.x = sqrt(1 + pow(ray.y/ray.x, 2));
        } else {
            unaryLength.x = DBL_MAX;
        }
        if (ray.y!=0) {
            unaryLength.y = sqrt(1 + pow(ray.x/ray.y, 2));
        } else {
            unaryLength.y = DBL_MAX;
        }
        Vector2i currentPosInMap;
        currentPosInMap.x = floor(tmp.x)/mapScaler;
        currentPosInMap.y = floor(tmp.y)/mapScaler;
        Vector2 currentLength;
        Vector2 direction;
        if (ray.x<0) {
            direction.x = -1;
            currentLength.x = (tmp.x/mapScaler - currentPosInMap.x) * unaryLength.x;
        } else {
            direction.x = 1;
            currentLength.x = (currentPosInMap.x+1 - tmp.x/mapScaler) * unaryLength.x;
        }
        if (ray.y<0) {
            direction.y = -1;
            currentLength.y = (tmp.y/mapScaler - currentPosInMap.y) * unaryLength.y;
        } else {
            direction.y = 1;
            currentLength.y = (currentPosInMap.y+1 - tmp.y/mapScaler) * unaryLength.y;
        }
        while (map[currentPosInMap.y][currentPosInMap.x]==0) {
            if (currentLength.x < currentLength.y) {
                currentPosInMap.x += direction.x;
                currentLength.x += unaryLength.x*direction.x;
            } else {
                currentPosInMap.y += direction.y;
                currentLength.y += unaryLength.y*direction.y;
            }
        }
        SDL_RenderDrawLineF(renderer, tmp.x, tmp.y, tmp.x+currentLength.x, tmp.y+currentLength.y);
        // SDL_RenderDrawLineF(renderer, player->position.x, player->position.y, tmp.x, tmp.y);
    }
}