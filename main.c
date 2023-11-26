#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "SDL2/SDL.h"

#define WINDOW_W 640
#define WINDOW_H 640
#define GAME_SIZE 600
#define STEP_SIZE 10
#define MVMT_SPEED 50
#define REFRESH_RATE 1000/30


struct Game {
  int window_w; 
  int window_h;
  int game_w;
  int game_h;
  int step;
  int mvmt_speed;
  SDL_Window* window;
  SDL_Renderer* win_ren;
};

void draw_game(struct Game *g){
      SDL_SetRenderDrawColor(g->win_ren, 255,255,255,255);
      SDL_RenderClear(g->win_ren);
      //render code here.
      SDL_RenderPresent(g->win_ren);
}

int main(void){
  if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_EVENTS)!=0){
    puts("Failed to init SDL");
    exit(EXIT_FAILURE);
  }
  struct Game game = {.window_w=WINDOW_W, 
    .window_h=WINDOW_H, 
    .game_w=GAME_SIZE, 
    .game_h=GAME_SIZE, 
    .step=STEP_SIZE,
    .mvmt_speed=MVMT_SPEED};

  SDL_CreateWindowAndRenderer(WINDOW_W, WINDOW_H, SDL_WINDOW_RESIZABLE, &game.window, &game.win_ren);
  if (!game.window || !game.win_ren) {
    printf("Failed to create window or renderer. Error: %s", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  SDL_SetWindowTitle(game.window, "snake");
  bool quit = false;
  SDL_Event event;
  int next_move_time = SDL_GetTicks() + game.mvmt_speed;

  while(!quit){
    int last_tick = SDL_GetTicks();
    while (SDL_PollEvent(&event)){
      if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_q){
        printf("Quit event\n");
        quit = true;
        break;
      }
      if (event.type == SDL_KEYDOWN){
        switch (event.key.keysym.sym){
          case SDLK_i:
            puts("debug info here");
            break;
            //arrow movements
          default:
            break;
        }
      }
    }

    if ( SDL_GetTicks() >= next_move_time) {
      //update
      //render
      draw_game(&game);
      int cur_delay = SDL_GetTicks() - last_tick;
      if (cur_delay < REFRESH_RATE) {
        SDL_Delay(cur_delay);
      }

      next_move_time = SDL_GetTicks() + game.mvmt_speed;
    }
  }
  SDL_DestroyRenderer(game.win_ren);
  SDL_DestroyWindow(game.window);
  exit(0);
}
