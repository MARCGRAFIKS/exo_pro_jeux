#ifndef __MAIN__H__
#define __MAIN__H__

#include <SDL.h>
#include <SDL_ttf.h>

#define STATUS_STATE_LIVES    0
#define STATUS_STATE_GAME     1
#define STATUS_STATE_GAMEOVER 2

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

typedef struct {
    float x, y;
    float dx, dy;
    short lives;
    char* name;
    bool isDead;
    int deadCounter;
    bool onLedge, slowingDown;
}Man;

typedef struct {
   int x, y;
   int w, h;
}Mur;

typedef struct {
    float scrollX;
    Man man;

    Mur mur[100];
    int time;
    int statusState;
    int texW, texH;
    // SDL_Texture* manTex;
    SDL_Texture* label1;
    TTF_Font* font;
    SDL_Renderer* renderer;
}GameState;

#endif