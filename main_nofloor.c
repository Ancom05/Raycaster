#include <SDL2/SDL_render.h>
#include <SDL2/SDL_hints.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_video.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linear_alg.h"

#define PI 3.14159f
#define WWIDTH 1024
#define WHEIGHT 768
#define MAPWIDTH 24
#define MAPHEIGHT 24
#define TEXWIDTH 256
#define TEXHEIGHT 256
#define BUFLEN WWIDTH*WHEIGHT*sizeof(int)

int map[MAPWIDTH][MAPHEIGHT]=
{
  {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,7,7,7,7,7,7,7,7},
  {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
  {4,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
  {4,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
  {4,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
  {4,0,4,0,0,0,0,5,5,5,5,5,5,5,5,5,7,7,0,7,7,7,7,7},
  {4,0,5,0,0,0,0,5,0,5,0,5,0,5,0,5,7,0,0,0,7,7,7,1},
  {4,0,6,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
  {4,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,7,1},
  {4,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8},
  {4,0,0,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,7,7,7,1},
  {4,0,0,0,0,0,0,5,5,5,0,0,5,5,5,5,7,7,7,7,7,7,7,1},
  {6,6,6,6,6,6,6,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
  {8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
  {6,6,6,6,6,6,0,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
  {4,4,4,4,4,4,0,4,4,4,6,0,6,2,2,2,2,2,2,2,3,3,3,3},
  {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
  {4,0,0,0,0,0,0,0,0,0,0,0,6,2,0,0,5,0,0,2,0,0,0,2},
  {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
  {4,0,6,0,6,0,0,0,0,4,6,0,0,0,0,0,5,0,0,0,0,0,0,2},
  {4,0,0,5,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
  {4,0,6,0,6,0,0,0,0,4,6,0,6,2,0,0,5,0,0,2,0,0,0,2},
  {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
  {4,4,4,4,4,4,4,4,4,4,1,1,1,2,2,2,2,2,2,3,3,3,3,3}
};
// {
//   {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
//   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//   {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
//   {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
//   {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
//   {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
//   {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
//   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//   {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//   {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//   {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//   {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//   {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//   {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//   {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//   {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
// };


typedef struct Player {
    Vector2 position;
    Vector2 direction;
    Vector2 screen;
} Player;

SDL_Color *colors;
SDL_Texture **screenTextures;
int buffer[BUFLEN];
const int bufLen = BUFLEN;

void castRays(SDL_Renderer *renderer, Player *player, int mapScaler, int *buffer, SDL_Surface **surfaces);
void initColors();
void initWindow(SDL_Window *window, SDL_Renderer *renderer);//TODO: da implementare
void writeBufferOnTexture(int *buffer, SDL_Texture *texture);
void drawPixel(int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a, int *buffer);
void drawVerticalLine(int x, int y1, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, int *buffer);
void initBuffer(int *buffer);
// typedef struct ScaledProjection {
//     int width;
//     int height;
//     int scale;
// } ScaledProjection;

int main(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING)==-1) {
        printf("Couldn't initialize SDL");
    }
    // initColors();
    //TODO: implementare un initTextures()
    int scale = 1;
    int mapScaler = 32;
    int fps = 60;  
    SDL_SetHint("SDL_HINT_RENDER_DRIVER", "opengl");
    SDL_Event keyboardEvent;
    Uint32 delayMs = (double)(((double)1.0f / (double)fps))*645;
    const Uint8 *pressedKeys = SDL_GetKeyboardState(NULL); 
    Vector2 initialPos = {11, 10};
    Vector2 initialDir = {0, -1};
    Vector2 screen = {0.66, 0}; //perpendicolare alla direzione e di stessa lunghezza, così da avere fov=90°
    Player player = {initialPos, initialDir, screen};
    SDL_Window *window;
    SDL_Renderer *renderer;
    screenTextures = malloc(2*sizeof(SDL_Texture*));
    window =  SDL_CreateWindow("Dumb Raycaster", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WWIDTH, WHEIGHT, SDL_WINDOW_OPENGL);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    for (int i = 0; i<2; i++) {
        //il flag SDL_TEXTUREACCES_STREAMING serve per poter usare i metodi LockTexture e UnlockTexture per poter scrivere sulla texture pixel per pixel
        screenTextures[i] = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WWIDTH, WHEIGHT);
        SDL_SetTextureBlendMode(screenTextures[i], SDL_BLENDMODE_NONE);
    }
    SDL_RaiseWindow(window);
    int isGameRunning = 1;
    SDL_Surface **surfaces = malloc(3*sizeof(SDL_Surface*));
    surfaces[0] = SDL_LoadBMP("assets/images/backrooms.bmp");
    surfaces[1] = SDL_LoadBMP("assets/images/backrooms_floor.bmp");
    surfaces[2] = SDL_LoadBMP("assets/images/backrooms_ceiling.bmp");
    for (int i = 0; i < 3; i++) {
        SDL_ConvertSurfaceFormat(surfaces[i], SDL_PIXELFORMAT_ARGB8888, 0);
        SDL_LockSurface(surfaces[i]);
    }
    Uint32 currentFrame = 0;
    initBuffer(buffer);
    while (isGameRunning) {
        Uint32 lastTick = SDL_GetTicks();
        SDL_RenderClear(renderer);
        while (SDL_PollEvent(&keyboardEvent)) {
            if (keyboardEvent.type == SDL_QUIT) {
                isGameRunning = 0;
            }
        }
        if (pressedKeys[SDL_SCANCODE_LEFT]) {
            rotateVector(&player.direction, -0.016); 
            rotateVector(&player.screen, -0.016);
        } 
        if (pressedKeys[SDL_SCANCODE_RIGHT]) {
            rotateVector(&player.direction, 0.016); 
            rotateVector(&player.screen, 0.016);
        }    
        if (pressedKeys[SDL_SCANCODE_UP]) {
            player.position.x+=player.direction.x/10;
            player.position.y+=player.direction.y/10;
        } 
        if (pressedKeys[SDL_SCANCODE_DOWN]) {
            player.position.x-=player.direction.x/10;
            player.position.y-=player.direction.y/10;
        }  
        // for (int i = 0; i < bufLen; i++) {
        //     buffer[i] = 0xFF;
        // }
        // castRays(renderer, &player, mapScaler, buffer);
        // writeBufferOnTexture(buffer, screenTextures[0]);
        // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        // for (int i = 0; i < MAPHEIGHT; i++) {
        //     for (int j = 0; j < MAPWIDTH; j++) {
        //         if (map[i][j]!=0) {
        //             SDL_Rect rect = {j, i, 1, 1};
        //             SDL_RenderDrawRect(renderer, &rect);
        //         }
        //     }
        // }
        // for (int i = 0; i < bufLen; i++) {
            // buffer[i] = alpha;
        // }
        // for (int i = 0; i < WHEIGHT; i++) {
        //     buffer[i*WWIDTH+WWIDTH-1] = 0xFF;
        // }
        castRays(renderer, &player, 0, buffer, surfaces);
        // SDL_RenderDrawPointF(renderer, player.position.x, player.position.y);
        // SDL_RenderDrawLineF(renderer, player.position.x, player.position.y, player.position.x + player.direction.x , player.position.y + player.direction.y );
        // SDL_RenderDrawLineF(renderer, player.position.x+player.direction.x, player.position.y + player.direction.y , player.position.x+player.direction.x+player.screen.x, player.position.y+player.direction.y+player.screen.y);
        // SDL_RenderDrawLineF(renderer, player.position.x+player.direction.x, player.position.y + player.direction.y , player.position.x+player.direction.x-player.screen.x, player.position.y+player.direction.y-player.screen.y);
        //FINE render 2D di prova
        // castRays(renderer, &player, mapScaler);
        writeBufferOnTexture(buffer, screenTextures[currentFrame%2]);
        SDL_RenderCopy(renderer, screenTextures[currentFrame%2], NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        Uint32 currentDelay = SDL_TICKS_PASSED(lastTick, SDL_GetTicks());
        for (int i = 0; i < 3; i++) {
            SDL_UnlockSurface(surfaces[i]);
            // SDL_FreeSurface(surfaces[i]);
        }
        currentFrame++;
        SDL_Delay(delayMs - currentDelay);
        // SDL_Delay(10);
    }
    free(surfaces);
    for (int i = 0; i < 2; i++) {
        SDL_DestroyTexture(screenTextures[i]);
    }
    free(screenTextures);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void castRays(SDL_Renderer *renderer, Player *player, int mapScaler, int *buffer, SDL_Surface **surfaces) {
    const int wcenter = WHEIGHT/2;//offsettato giu di 50 per avere una visuale un po' piu' bassa
    
    Uint32 **surfPixels = malloc(3*sizeof(Uint32*));
    for (int i = 0; i < 3; i++) {
        surfPixels[i] = surfaces[i]->pixels;
    }
    //floor casting
    // for (int i = wcenter+1; i < WHEIGHT; i++) {
    //     Vector2 mostLeftRay;
    //     mostLeftRay.x = player->direction.x - player->screen.x;
    //     mostLeftRay.y = player->direction.y - player->screen.y;
    //     Vector2 mostRightRay;
    //     mostRightRay.x = player->direction.x + player->screen.x;
    //     mostRightRay.y = player->direction.y + player->screen.y;
    //     //altezza rispetto all'orizzonte a centro schermo
    //     int currentHeight = i - wcenter;
    //     //wcenter e' l'orizzonte, quindi e' il punto di massimo profondita, allora per calcolare la profondita del punto corrente lo dividi con l'altezza di quest'ultimo rispetto all'orizzonte
    //     double distance =  (double)wcenter/currentHeight;
    //     //distanza di cui spostarsi ogni volta sulla texture
    //     Vector2 floorStep;
    //     floorStep.x = distance * (mostRightRay.x - mostLeftRay.x)/(double)WWIDTH;
    //     floorStep.y = distance * (mostRightRay.y - mostLeftRay.y)/(double)WWIDTH;
    //     //posizione reale sulla texture
    //     Vector2 currentPosFloor;
    //     currentPosFloor.x = player->position.x + distance * mostLeftRay.x;
    //     currentPosFloor.y = player->position.y + distance * mostLeftRay.y;
    //     Vector2i floorIndex;
    //     Vector2i pixelCoordsInTexture;
    //     for (int j = 0; j < WWIDTH; j++) {
    //         floorIndex.x = (int)currentPosFloor.x;
    //         floorIndex.y = (int)currentPosFloor.y;
    //         pixelCoordsInTexture.x = (int)((currentPosFloor.x - floorIndex.x) * TEXWIDTH) & (TEXWIDTH-1);
    //         pixelCoordsInTexture.y = (int)((currentPosFloor.y - floorIndex.y) * TEXHEIGHT) & (TEXHEIGHT-1);
    //         currentPosFloor.x += floorStep.x;
    //         currentPosFloor.y += floorStep.y;
    //         // Uint32 color;
    //         buffer[i*WWIDTH+j] =  surfPixels[1][WWIDTH*pixelCoordsInTexture.y + pixelCoordsInTexture.x];
    //         buffer[(WHEIGHT-i-1)*WWIDTH+j] = surfPixels[2][WWIDTH*pixelCoordsInTexture.y + pixelCoordsInTexture.x];
    //     }
    // }
    //walls casting
    for (int i = 0; i < WWIDTH; i++) {
        double cameraX = 2 * i / (double)WWIDTH - 1;
        //vettore che rappresenta la direzione del raggio
        Vector2 ray;
        ray.x = player->direction.x + player->screen.x * cameraX;
        ray.y = player->direction.y + player->screen.y * cameraX;
        //vettore che rappresenta il raggio vero e proprio
        Vector2 tmp;
        tmp.x = ray.x + player->position.x;
        tmp.y = ray.y + player->position.y;
        //vettore che rappresenta la distanza percorsa per ogni unita' in X e Y
        Vector2 unaryLength;
        unaryLength.x = ray.x!=0 ? sqrt(1 + pow(ray.y/ray.x, 2)) : DBL_MAX;
        unaryLength.y = ray.y!=0 ? sqrt(1 + pow(ray.x/ray.y, 2)) : DBL_MAX;
        //vettore che rappresenta la posizione corrente nella tilemap
        Vector2i currentPosInMap;
        currentPosInMap.x = floor(tmp.x);
        currentPosInMap.y = floor(tmp.y);
        Vector2 currentLength;
        Vector2i direction;
        //calcolo della distanza del punto dal bordo orizzontale e verticale
        if (ray.x<0) {
            direction.x = -1;
            currentLength.x = (player->position.x - currentPosInMap.x) * unaryLength.x;
        } else {
            direction.x = 1;
            currentLength.x = (currentPosInMap.x+1.0 - player->position.x) * unaryLength.x;
        }
        if (ray.y<0) {
            direction.y = -1;
            currentLength.y = (player->position.y - currentPosInMap.y) * unaryLength.y;
        } else {
            direction.y = 1;
            currentLength.y = (currentPosInMap.y+1.0 - player->position.y) * unaryLength.y;
        }
        int side;
        //dda
       do {
            if (currentLength.x < currentLength.y) {
                currentPosInMap.x += direction.x;
                currentLength.x += unaryLength.x;
                side=1;
            } else {
                currentPosInMap.y += direction.y;
                currentLength.y += unaryLength.y;
                side=2;
            }
        } while (map[currentPosInMap.y][currentPosInMap.x]==0);
        double wallDist;
        double positionOnTexture;
        if (side==1) {
            wallDist = currentLength.x - unaryLength.x;
        } else {
            wallDist = currentLength.y - unaryLength.y;
        }
        wallDist = wallDist*cos(dotProduct(player->screen, ray)/(norm(ray)*norm(player->screen)));
        
        if (side==1) {
            positionOnTexture = player->position.y + wallDist * ray.y;
        } else {
            positionOnTexture = player->position.x + wallDist * ray.x;
        }
        positionOnTexture -= floor(positionOnTexture);
        positionOnTexture = positionOnTexture*(double)TEXWIDTH;
        if ((side==1 && ray.x>0) || (side==2 && ray.y<0)) positionOnTexture = TEXWIDTH - positionOnTexture -1; 
        int wallHeight = WHEIGHT/wallDist;
        int halfWallHeight = wallHeight/2;
        if (halfWallHeight>wcenter-1) {
            halfWallHeight = wcenter-1;
        }
        if (halfWallHeight < 0) {
            halfWallHeight = 0;
        }
        int j=0;
        for (; j < wcenter - halfWallHeight; j++) {
            // buffer[j*WWIDTH+i] = 0x0000CC;

            drawPixel(i, j, 0, 0, 0xCC, 255, buffer);
        }
        double step = (double)TEXHEIGHT/wallHeight;
        double texY = 0;
        for (j = wcenter - halfWallHeight; j < wcenter+halfWallHeight; j++) {
            int texYIndex = (int)texY & (TEXHEIGHT-1);
            texY+=step;
            int index = (int)texYIndex*surfaces[0]->w+positionOnTexture;
            Uint32 color = surfPixels[0][index];
            if (side==1) {
                color = (((color>>24) & 0xFF)>>1)<<24 | (((color>>16) & 0xFF)>>1)<<16 | (((color>>8) & 0xFF)>>1)<<8 | (color & 0xFF)>>1;
            }
            buffer[j*WWIDTH+i] = color;
            // drawPixel(i, j, surfPixels[startingIndex+3], surfPixels[startingIndex+2], surfPixels[startingIndex+1], surfPixels[startingIndex], buffer);
        }
        // for (; j < wcenter+halfWallHeight; j++) {
        //     buffer[j*WWIDTH+i] = 0xFFFFFF;
        // }
        // for (; j < wcenter+halfWallHeight*3; j++) {
        //     buffer[j*WWIDTH+i] = 0xCCCCCC/side;
        // }
        for (; j < WHEIGHT; j++) {
            drawPixel(i, j, 0, 0, 0, 255, buffer);
            // buffer[j*WWIDTH+i] = 0x000000;
        }
        // SDL_SetRenderDrawColor(renderer, 0, 0, 150, 255);
        // SDL_RenderDrawLine(renderer, i, 0, i, wcenter-halfWallHeight);
        // SDL_SetRenderDrawColor(renderer, colors[map[currentPosInMap.y][currentPosInMap.x]-1].r/side, colors[map[currentPosInMap.y][currentPosInMap.x]-1].g/side, colors[map[currentPosInMap.y][currentPosInMap.x]-1].b/side, 255);
        // SDL_RenderDrawLine(renderer, i, wcenter-halfWallHeight, i, wcenter+halfWallHeight);
        // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        // SDL_RenderDrawLine(renderer, i, wcenter+halfWallHeight, i, WHEIGHT-1);
        // SDL_RenderDrawLineF(renderer, player->position.x, player->position.y, tmp.x, tmp.y);
    }
}

void initColors() {
    colors = malloc(4*sizeof(SDL_Color));
    colors[0].r = 255;
    colors[0].g = 0;
    colors[0].b = 0;
    colors[1].r = 10;
    colors[1].g = 250;
    colors[1].b = 0;
    colors[2].r = 0;
    colors[2].g = 10;
    colors[2].b = 250;
    colors[3].r = 255;
    colors[3].g = 255;
    colors[3].b = 255;
    for (int i = 0; i < 4; i++) {
        colors[i].a = 255;
    }
}

void writeBufferOnTexture(int *buffer, SDL_Texture *texture) {
    int pitch;
    void *pixels;
    SDL_LockTexture(texture, NULL, &pixels, &pitch);
    // memset(pixels, 0xFF, sizeof(void*)*BUFLEN);
    memcpy(pixels, buffer, bufLen);
    SDL_UnlockTexture(texture);
}


void drawPixel(int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a, int *buffer) {
    buffer[y*WWIDTH+x] = b;
    buffer[y*WWIDTH+x] |= g<<8;
    buffer[y*WWIDTH+x] |= r<<16;
    buffer[y*WWIDTH+x] |= a<<24;
}



void initBuffer(int *buffer) {
    int alpha = 0xFF<<24;
    for (int i = 0; i < bufLen; i++) {
        buffer[i] = alpha;
    }
}