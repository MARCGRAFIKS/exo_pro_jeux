#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <time.h>
#include <math.h>

#define GRAVITY   0.35;  


 typedef struct {
	 float x, y;
     float dx, dy;
	 short life;
    int onLedge;
	 char* name;
 }Man;
 typedef struct {
    int x, y;
 }Star;
 typedef struct {
    int x, y, w, h;
 }Ledge;

 typedef struct {
    Man man;
    Star star[100];
    Ledge ledge[100];
    SDL_Texture* starTex;
    SDL_Texture* manFrames[2];
    SDL_Texture* brickTex;
    SDL_Renderer* renderer;
 }GameState;

 void loadGame(GameState* game) {
    SDL_Surface* starSurf = NULL;
    starSurf = IMG_Load("image/star.png");
   if(starSurf == NULL) {
    printf("Error de load image!!%s, IMG Error%s","image/star.png", IMG_GetError());
    SDL_Quit();
    exit (1);
   }
    game->starTex = SDL_CreateTextureFromSurface(game->renderer, starSurf);
    SDL_FreeSurface(starSurf);

    starSurf = IMG_Load("image/man_lta.png");
    if(starSurf == NULL) {
    printf("Error de load image!!%s, IMG Error%s","image/star.png", IMG_GetError());
    SDL_Quit();
    exit (1);
   }
    game->manFrames[0] = SDL_CreateTextureFromSurface(game->renderer, starSurf);

    SDL_FreeSurface(starSurf);
    starSurf = IMG_Load("image/man_ltb.png");
    if(starSurf == NULL) {
    printf("Error de load image!!%s, IMG Error%s","image/star.png", IMG_GetError());
    SDL_Quit();
    exit (1);
   }
    game->manFrames[1] = SDL_CreateTextureFromSurface(game->renderer, starSurf);
    SDL_FreeSurface(starSurf);

    starSurf = IMG_Load("image/brick.png");
    if(starSurf == NULL) {
    printf("Error de load image!!%s, IMG Error%s","image/star.png", IMG_GetError());
    SDL_Quit();
    exit (1);
   }
    game->brickTex= SDL_CreateTextureFromSurface(game->renderer, starSurf);
    SDL_FreeSurface(starSurf);

     game->man.x = 220;
     game->man.y = 140;
     game->man.dx = 0;
     game->man.dy = 0;
     game->man.onLedge = 0;

     for(int i(0); i<100; ++i) {
       game->star[i].x = rand() % 640;
       game->star[i].y = rand() % 480;
     }
    for(int i(0); i<100; ++i) {
        game->ledge[i].w = 256;
        game->ledge[i].h = 64;
        game->ledge[i].x = i*256;
        game->ledge[i].y = 400;
    }
    game->ledge[99].x = 350;
    game->ledge[99].y = 200;

    game->ledge[98].x = 350;
    game->ledge[98].y = 350;
 }

 bool proccessEvent(SDL_Window* window, GameState* game) {
    int done = 0;
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
	  switch(event.type) {
          case SDL_WINDOWEVENT_CLOSE: {
	    if(window) {
              SDL_DestroyWindow(window);
	      window = NULL;
	    }
	  }
	   break;
	  case SDL_KEYDOWN: {
	   switch(event.key.keysym.sym) {
          case SDLK_b:
		  done = 1;
		  break;
          case SDLK_UP:
          if(game->man.onLedge){
             game->man.dy = -8;
             game->man.onLedge = 0;
          }
          break;
	   }
	 }
	  break;
	  case SDL_QUIT:
	  done = 1;
	  break;
	}
      }
    
 const Uint8* state = SDL_GetKeyboardState(NULL);
  
  //Walking
  if(state[SDL_SCANCODE_LEFT])
  {
    game->man.x -= 10;
  }
  if(state[SDL_SCANCODE_RIGHT])
  {
    game->man.x += 10;
  }

    return done;
 }

 void proccess(GameState* game) {
    Man* man = &game->man;
    man->y += man->dy;

    man->dy += GRAVITY;
 }
  
 void collisionDetect(GameState *game)
{
   for(int i = 0; i < 100; i++)
  {
    float mw = 48, mh = 48;
    float mx = game->man.x, my = game->man.y;
    float bx = game->ledge[i].x, by = game->ledge[i].y, bw = game->ledge[i].w, bh = game->ledge[i].h;

    if(mx+mw/2 > bx && mx+mw/2<bx+bw)
    {
      //are we bumping our head?
      if(my < by+bh && my > by)
      {
        //correct y
        game->man.y = by+bh;
        my = by+bh;
        
        //bumped our head, stop any jump velocity
        game->man.dy = 0;
        game->man.onLedge = 1;
      }
    }
    if(mx+mw > bx && mx<bx+bw)
    {
      //are we landing on the ledge
      if(my+mh > by && my < by)
      {
        //correct y
        game->man.y = by-mh;
        my = by-mh;
        
        //landed on this ledge, stop any jump velocity
        game->man.dy = 0;
        game->man.onLedge = 1;
      }
    }
  
    if(my+mh > by && my<by+bh)
    {
      //rubbing against right edge
      if(mx < bx+bw && mx+mw > bx+bw)
      {
        //correct x
        game->man.x = bx+bw;
        mx = bx+bw;
        
      }
      //rubbing against left edge
      else if(mx+mw > bx && mx < bx)
      {
        //correct x
        game->man.x = bx-mw;
        mx = bx-mw;
        
      }
    }
  }
}

 void doRenderer(GameState* game) {
        SDL_SetRenderDrawColor(game->renderer, 128, 128, 255, 255);
           //clear the screen to blue
        SDL_RenderClear(game->renderer);
        SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
        for(int i(0); i< 100; ++i) {
           SDL_Rect ledgeRect = {game->ledge[i].x, game->ledge[i].y, game->ledge[i].w, game->ledge[i].h};
           SDL_RenderCopy(game->renderer, game->brickTex, NULL, &ledgeRect);
        }
        SDL_Rect rect = {game->man.x, game->man.y, 48, 48};
        SDL_RenderCopyEx(game->renderer, game->manFrames[0], NULL, &rect, 0, NULL, SDL_FLIP_NONE);
    //  for(int i(0); i<100; ++i){
    //      SDL_Rect starRect = {game->star[i].x, game->star[i].y, 64, 64};
    //      SDL_RenderCopy(game->renderer, game->starTex, NULL, &starRect);
    //  }
        SDL_RenderPresent(game->renderer);
 }
  

int main(int argc, char* argv[]) {
    GameState game;
    SDL_Window* window = NULL;
    SDL_Renderer*renderer = NULL;
    srand(time(NULL));

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Game Window", 
		               SDL_WINDOWPOS_UNDEFINED,
		               SDL_WINDOWPOS_UNDEFINED,
			       640,
			       480,
			       0
		              );
   renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    game.renderer = renderer;
    bool done = 0;
    loadGame(&game);
    while(!done) {
       done = proccessEvent(window, &game);
       proccess(&game);
       collisionDetect(&game); 
       doRenderer(&game);
        // SDL_Delay(100);
    }
   SDL_DestroyTexture(game.starTex); 
   SDL_DestroyWindow(window);
   SDL_DestroyRenderer(renderer);
   SDL_Quit();

 return 0;
}

