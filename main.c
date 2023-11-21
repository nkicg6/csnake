#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "SDL2/SDL.h"


typedef enum {
  UP,
  DOWN,
  LEFT,
  RIGHT,
  STOPPED,
} SnakeMvmtDirection;


//take an array of rects, 
void draw_snake(SDL_Renderer *rend, SDL_Rect *rect){
}

void draw_game(SDL_Renderer *rend, SDL_Rect *rect, int r, int g, int b){
  SDL_SetRenderDrawColor(rend, r, g, b, 255);
  SDL_RenderDrawRect(rend, rect);
}


const int WIN_WIDTH = 640;
const int WIN_HEIGHT = 480; 
const int TARGET_TICK = 1000/60; // 60fps

int main(void) {
  if (0!=SDL_Init(SDL_INIT_EVENTS|SDL_INIT_VIDEO|SDL_INIT_TIMER)){
    printf("Failed to start SDL, error: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }else{
    printf("Initialized SDL2\n");
  }

  SDL_Event event;

  SDL_Window *window = SDL_CreateWindow("Snake", 0, 0, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_RESIZABLE);

  if(!window){
    printf("Failed to create window, SDL Error: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  SDL_Renderer *win_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);

  if(!win_renderer){
    printf("Failed to create win_renderer, SDL Error: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  int head_pos_x = 100;
  int head_pos_y = 25;
  int step = 5;

  // snake, fixed length for now.
  int tailLen = 9;
  SDL_Rect snake[tailLen];
  SDL_Rect head = {.x = head_pos_x, .y = head_pos_y, .w = 25, .h = 25};
  snake[0] = head;
  for (int i = 1; i < tailLen; i++){
    snake[i] = (SDL_Rect){.x = head_pos_x-step*i, .y=head_pos_y, .w=25,.h=25};
  }

  SDL_Rect game_rect = {.x = 25, .y = 0, .w = 600, .h = 480}; 

  SnakeMvmtDirection snake_direction = STOPPED;

  bool quit = false;

  while(!quit){
    int last_tick = SDL_GetTicks();

    while (SDL_PollEvent(&event)){
      if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_q){
        printf("Quit event\n");
        quit = true;
        break;
      }

      switch (event.key.keysym.sym){
        case SDLK_i:
          //debug info
          printf("headx = %d, heady = %d\n", head.x, head.y);
          printf("game_rect width = %d, game_rect height = %d\n", game_rect.x+game_rect.w, game_rect.y+game_rect.h);
          break;

        case SDLK_LEFT:
          if (snake_direction == RIGHT) {
            printf("Can't turn like that!\n");
            break;
          }
          snake_direction = LEFT;
          printf("left button\n");
          break;

        case SDLK_RIGHT:
          if (snake_direction == LEFT) {
            printf("Can't turn like that!\n");
            break;
          }
          snake_direction = RIGHT;
          printf("right button\n");
          break;

        case SDLK_UP:
          if (snake_direction == DOWN) {
            printf("Can't turn like that!\n");
            break;
          }
          snake_direction = UP;
          printf("up button\n");
          break;

        case SDLK_DOWN:
          if (snake_direction == UP) {
            printf("Can't turn like that!\n");
            break;
          }
          snake_direction = DOWN;
          printf("down button\n");
          break;

        case SDLK_SPACE:
          snake_direction = STOPPED;
          printf("pause\n");
          break;

        default:
          break;
      }
    }
    // update events
    switch(snake_direction){
      case UP:
        if (snake[0].y-step < game_rect.y) {
          printf("Exceeds bounds\n");
          snake_direction = STOPPED;
          break;
        }
        snake[0].y -= step;
        break;

      case DOWN:
        if (snake[0].y+snake[0].h+step > game_rect.y+game_rect.h){
          printf("Exceeds bounds\n");
          snake_direction = STOPPED;
          break;
        }
        snake[0].y += step;
        break;

      case LEFT:
        if (snake[0].x-step < game_rect.x){
          printf("Exceeds bounds\n");
          snake_direction = STOPPED;
          break;
        }
        snake[0].x -= step;
        break;

      case RIGHT:
        if (snake[0].x+snake[0].w+step > game_rect.x+game_rect.w){
          printf("Exceeds bounds\n");
          snake_direction = STOPPED;
          break;
        }
        snake[0].x += step;
        break;

      case STOPPED:
        break;
    }
    //needs to go in update... 
    if (snake_direction != STOPPED){
      for (int i=tailLen-1; i>0; i--){
        snake[i] = snake[i-1];
      }
    }

    //render

    int cur_delay = SDL_GetTicks() - last_tick;
    if (cur_delay < TARGET_TICK) {
      SDL_Delay(cur_delay);
    }
    SDL_SetRenderDrawColor(win_renderer, 255,255,255,255);
    SDL_RenderClear(win_renderer);
    SDL_SetRenderDrawColor(win_renderer, 255,25,255,255);
    for (int i=0;i<tailLen;i++){
      SDL_RenderFillRect(win_renderer, &snake[i]);
    }
    draw_game(win_renderer, &game_rect, 200, 200, 200);
    SDL_RenderPresent(win_renderer);
    last_tick = SDL_GetTicks();
  }

  //cleanup
  SDL_DestroyRenderer(win_renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  exit(0);
}
