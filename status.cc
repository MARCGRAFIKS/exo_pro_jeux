#include "status.h"
#include <stdio.h>

void init_statues_lives(GameState* game) {
   char str[123] = " ";
   sprintf(str, "  x%d", game->man.lives); 
   SDL_Color color = {0xFF, 0xFF, 0xFF};
   SDL_Surface* surf = TTF_RenderText_Blended(game->font, str, color);
   game->label1 = SDL_CreateTextureFromSurface(game->renderer, surf);
   game->texW = surf->w;
   game->texH = surf->h;
   SDL_FreeSurface(surf);
}

void draw_status_lives(GameState* game) {
    
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
    SDL_RenderClear(game->renderer);

    SDL_SetRenderDrawColor(game->renderer, 0, 255, 255, 255);
    SDL_Rect rect = {(SCREEN_WIDTH - game->texW)/2 - game->texW+20, (SCREEN_HEIGHT - game->texH)/2+20, 30, 30};
    SDL_RenderFillRect(game->renderer, &rect);

    SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
    SDL_Rect Rrect = {(SCREEN_WIDTH - game->texW)/2, (SCREEN_HEIGHT - game->texH)/2, game->texW, game->texH};
    SDL_RenderCopy(game->renderer, game->label1, NULL, &Rrect);

}

void shutdown_status_lives(GameState* game) {
    if(game->label1!= NULL) {
        SDL_DestroyTexture(game->label1);
        game->label1 = NULL;
    }
}