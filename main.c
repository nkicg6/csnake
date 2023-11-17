#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "SDL2/SDL.h"


void draw_snake(SDL_Renderer* rend, SDL_Rect rect){
  SDL_SetRenderDrawColor(rend, 255,255,255,255);
  SDL_RenderClear(rend);
  SDL_SetRenderDrawColor(rend, 255,25,255,255);
  SDL_RenderFillRect(rend, &rect);
  SDL_RenderDrawRect(rend, &rect);
  SDL_RenderPresent(rend);
}


//TODO 
//Create playable window size with grid (min game size), re-size
//using https://wiki.libsdl.org/SDL2/SDL_GetRendererOutputSize
//rest will be score and other info
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

  SDL_Window *window = SDL_CreateWindow("Snake", 0,0,640,480, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);

  if(!window){
    printf("Failed to create window, SDL Error: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
  if(!renderer){
    printf("Failed to create renderer, SDL Error: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  int head_pos_x = 25;
  int head_pos_y = 25;
  int step_x = 25;
  int step_y = 25;
  // snake start
  SDL_Rect head;
  head.x = head_pos_x;
  head.y = head_pos_y;
  head.w = 25;
  head.h = 25;

  draw_snake(renderer, head);

  while(!quit){
    SDL_Delay(50);
    while (SDL_PollEvent(&event)){

      switch (event.type) {

        case SDL_KEYDOWN:
          switch (event.key.keysym.sym){
            case SDLK_q:
              printf("Quit Key\n");
              quit = true;
              SDL_DestroyWindow(window);
              break;
            case SDLK_LEFT:
              printf("Left key\n");
              head.x -= step_x;
              draw_snake(renderer, head);
              break;
            case SDLK_RIGHT:
              printf("Right key\n");
              head.x += step_x;
              draw_snake(renderer, head);
              break; 
            case SDLK_UP:
              printf("Up key\n");
              head.y -= step_y;
              draw_snake(renderer, head);
              break;            
            case SDLK_DOWN:
              printf("Down key\n");
              head.y += step_y;
              draw_snake(renderer, head);
              break;            
            default:
              printf("Unknown key pressed\n");
              break;
          }
          break;

        case SDL_QUIT:
          printf("Quit event\n");
          quit = true;
          SDL_DestroyWindow(window);
          break;

        default:
          break;

      }

    }
  }
  SDL_Quit();
  exit(0);
}
