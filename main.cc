#include <stdio.h>
#include <SDL_image.h>
#include <cmath>
#include "main.h"
#include "status.h"

#define GRAVITE 0.35f


void loadGame(GameState* game) {

    game->font = TTF_OpenFont("font/Crazy-Pixel.ttf", 48);
    if(!game->font) {
        printf("Errpr to load text:%s\n", TTF_GetError());
        return;
    }


    game->label1 = NULL;
    game->man.x = 123.f;
    game->man.y = 93.f;
    game->man.dx = 0;
    game->man.dy = 0;
    game->man.lives = 3;
    game->man.onLedge = false;
    game->man.slowingDown = false;
    game->man.isDead = false;
    game->man.deadCounter = -1;

    game->statusState = STATUS_STATE_LIVES;
    init_statues_lives(game);
    game->scrollX = 0;

    for(int i(0); i<100; ++i) { 
        game->mur[i].w = 150;
        game->mur[i].h = 30;
        game->mur[i].x = 250*i;
        if(i ==0)
        game->mur[i].y = 420;
        else
        game->mur[i].y = 420-rand()%200;
    }    

    game->mur[99].x =150;
    game->mur[99].y = 260;

    game->time = 0;
}
void proccess(GameState* game) {
    game->time++;

    if(game->time > 120) { 
        game->statusState = STATUS_STATE_GAME;
        shutdown_status_lives(game);    
    }

    if(game->statusState == STATUS_STATE_GAME){ 
    if(!game->man.isDead) { 
    Man* man = &game->man;
    man->x += man->dx;
    man->y += man->dy;

    man->dy += GRAVITE;
    }
    if(game->man.isDead && game->man.deadCounter < 0){
        game->man.deadCounter = 120;
    }
    if(game->man.deadCounter > 0){
        game->man.deadCounter--;
        if(game->man.deadCounter < 0){
	      game->man.lives--;
	    if(game->man.lives >=0) {
           game->statusState = STATUS_STATE_LIVES;
           init_statues_lives(game);
           game->time = 0;
            game->man.x = 123.f;
            game->man.y = 93.f;
            game->man.dx = 0;
            game->man.dy = 0;
            game->man.onLedge = false;
            game->man.isDead = false;
	      }else {
              //  init_game_over(game);
               game->statusState = STATUS_STATE_GAMEOVER;
               game->time = 0;
	      }
        }
     }
   }
   game->scrollX = -game->man.x+150;
   if(game->scrollX > 0) {
      game->scrollX = 0;
   }
}

 bool proccessEvent(SDL_Window* window, GameState* game) {
    bool run = false;
     SDL_Event ev;
        while(SDL_PollEvent(&ev)) {
            switch(ev.type) {
                case SDL_WINDOWEVENT_CLOSE:
                if(window) {
                    SDL_DestroyWindow(window);
                    window = NULL;
                }
                break;
                case SDL_KEYDOWN:
                switch(ev.key.keysym.sym) {
                    case SDLK_b:
                    run = true;
                    break;
                    case SDLK_UP:
                    if(game->man.onLedge) {
                       game->man.dy = -8;  
                       game->man.onLedge = false; 
                    }
                    break;
                }
                break;
                case SDL_QUIT:
                run = true;
                break;
            }
        }
        const Uint8* state = SDL_GetKeyboardState(NULL);
         if(state[SDL_SCANCODE_UP]) {
            game->man.dy = -12;
        }
        
        if(state[SDL_SCANCODE_LEFT]) {
            game->man.dx -= 0.5f;
            if(game->man.dx < -6) {
                game->man.dx = -6;
                game->man.slowingDown = false;
            }
        }else
        if(state[SDL_SCANCODE_RIGHT]) {
            game->man.dx += 0.5f;
            if(game->man.dx > 6) {
                game->man.dx = 6;
                game->man.slowingDown = false;
            }
        }else {
              game->man.dx += 0.8f;
            game->man.slowingDown = true;
            if(fabsf(game->man.dx > 0.1)){
                game->man.dx = 0.0f;
            }
        }
       
     return run;   
 }

 void collisionDetect(GameState* game) {
   
   for(int i(0); i<100; ++i) { 
       float mw = 64, mh = 64;
       float mx = game->man.x, my = game->man.y;
       float bx = game->mur[i].x, by = game->mur[i].y, bw = game->mur[i].w, bh = game->mur[i].h;

       if(mx+mw/2 < bx+bw && mx+mw/2 > bx) {
           if(my < by+bh && my > by && game->man.dy < 0) {
            game->man.y = by + bh;
            my = by + bh;
            game->man.dy = 0;
            game->man.onLedge = true;
           }
       }
       if(mx+mw > bx && mx < bx+bw) {
          if(my+mh > by && my < by && game->man.dy > 0) {
             game->man.y = by - mh;
             my = by - mh;
             game->man.dy = 0;
             game->man.onLedge = true;
          }
       }
       if(my+mh > by && my < by+bh) {
         if(mx < bx+bw && mx+mw > bx+bw && game->man.dx < 0) {
            game->man.x = bx + bw;
            mx = bx + bw;
            game->man.dx = 0;
         }else
         if(mx+mw > bx && mx < bx && game->man.dx > 0) {
            game->man.x = bx - mw;
            my = bx - mw;
            game->man.dx = 0;
         }
       }
    }
 }

 void doRenderer(GameState* game) {
    if(game->statusState == STATUS_STATE_LIVES) {
        draw_status_lives(game);
    }else if(game->statusState == STATUS_STATE_GAME) { 
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 255, 255);
    SDL_RenderClear(game->renderer);

    SDL_SetRenderDrawColor(game->renderer, 0, 255, 255, 255);
    SDL_Rect rect = {game->scrollX+game->man.x, game->man.y, 64, 64};
    SDL_RenderFillRect(game->renderer, &rect);
    SDL_SetRenderDrawColor(game->renderer, 255, 0, 255, 255);
     for(int i(0); i<100; ++i) { 
      SDL_Rect murRect = {game->scrollX+game->mur[i].x, game->mur[i].y, game->mur[i].w, game->mur[i].h};
      SDL_RenderFillRect(game->renderer, &murRect);
    }
   }
    SDL_RenderPresent(game->renderer);
 }

int main( int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    GameState game;

    if(SDL_Init(SDL_INIT_VIDEO)<0) {
        printf("error init sdl%s", SDL_GetError());
        SDL_Quit();
        exit (1);
    }else {
        if(IMG_Init(IMG_INIT_PNG) < 1) {
        printf("error init png%s", IMG_GetError());
        IMG_Quit();
        exit (1);
        }
    }

    TTF_Init();
    window = SDL_CreateWindow("GAME_CONSOL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);  
    game.renderer = renderer;
    
    loadGame(&game);
    bool run = false;

    while(!run) {
        run = proccessEvent(window, &game);
        collisionDetect(&game);
        proccess(&game);
        doRenderer(&game);
        SDL_Delay(10);
    }

    
    if(game.label1 != NULL) {
        SDL_DestroyTexture(game.label1);
    }
    TTF_CloseFont(game.font);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    if(game.renderer != NULL) {
        SDL_DestroyRenderer(game.renderer);
    }
    
    return 0;
}
