#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "SDL2/SDL.h"


void draw_snake(SDL_Renderer *rend, SDL_Rect *rect){
  SDL_SetRenderDrawColor(rend, 255,255,255,255);
  SDL_RenderClear(rend);
  SDL_SetRenderDrawColor(rend, 255,25,255,255);
  SDL_RenderFillRect(rend, rect);
  SDL_RenderDrawRect(rend, rect);
}

void draw_game(SDL_Renderer *rend, SDL_Rect *rect, int r, int g, int b){
  SDL_SetRenderDrawColor(rend, r, g, b, 255);
  SDL_RenderDrawRect(rend, rect);
}


const int WIN_WIDTH = 640;
const int WIN_HEIGHT = 480; 

int main(void) {
  int init = SDL_Init(SDL_INIT_EVENTS|SDL_INIT_VIDEO|SDL_INIT_TIMER);
  if (0!=init){
    printf("Failed to start SDL, exit code: %d. Exiting\n", init);
    exit(EXIT_FAILURE);
  }else{
    printf("Initialized SDL2\n");
  }
  SDL_Event event;
  bool quit = false;

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

  int head_pos_x = 25;
  int head_pos_y = 25;
  int step_x = 25;
  int step_y = 25;

  // snake
  SDL_Rect head = {.x = head_pos_x, .y = head_pos_y, .w = 25, .h = 25};

  SDL_Rect game_rect = {.x = 25, .y = 0, .w = 600, .h = 480}; 

  while(!quit){
    SDL_Delay(50);
    while (SDL_PollEvent(&event)){
      switch (event.type) {
        case SDL_KEYDOWN:
          switch (event.key.keysym.sym){
            case SDLK_i:
              printf("headx = %d, heady = %d\n", head.x, head.y);
              printf("game_rect width = %d, game_rect height = %d\n", game_rect.x+game_rect.w, game_rect.y+game_rect.h);
              break;
            case SDLK_q:
              printf("Quit Key\n");
              quit = true;
              break;
            case SDLK_LEFT:
              printf("Left key\n");
              if ((head.x - step_x) < game_rect.x){
                printf("Exceeds bounds\n");
                break;
              }
              head.x -= step_x;
              break;
            case SDLK_RIGHT:
              printf("Right key\n");
              if (((head.x+head.w) + step_x) > (game_rect.x+game_rect.w)){
                printf("Exceeds bounds\n");
                break;
              }
              head.x += step_x;
              break; 
            case SDLK_UP:
              printf("Up key\n");
              if ((head.y - step_y) < game_rect.y) {
                printf("Exceeds bounds\n");
                break;
              }
              head.y -= step_y;
              break;            
            case SDLK_DOWN:
              printf("Down key\n");
              if ((head.y + head.h + step_y) > game_rect.y + game_rect.h){
                printf("Exceeds bounds\n");
                break;
              }
              head.y += step_y;
              break;            
            default:
              printf("key pressed\n");
              break;
          }
          break;
        case SDL_WINDOWEVENT:
          //could handle resizes more gracefully here...
          //case SDL_WINDOWEVENT_RESIZED:
          //case SDL_WINDOWEVENT_SIZE_CHANGED:
          printf("Window size changed\n");
          break;
        case SDL_QUIT:
          printf("Quit event\n");
          quit = true;
          break;

        default:
          break;
      }
    }
    draw_snake(win_renderer, &head);
    draw_game(win_renderer, &game_rect, 200, 200, 200);
    SDL_RenderPresent(win_renderer);
  }
  SDL_DestroyRenderer(win_renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  exit(0);
}
