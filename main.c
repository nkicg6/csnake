#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "SDL2/SDL.h"

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

  SDL_SetRenderDrawColor(renderer, 255,255,255,255);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 255,25,255,255);
  SDL_RenderFillRect(renderer, &head);
  SDL_RenderDrawRect(renderer, &head);
  SDL_RenderPresent(renderer);

  while(!quit){
    SDL_Delay(250);
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
              break;
            case SDLK_RIGHT:
              printf("Right key\n");
              break;            
            case SDLK_UP:
              printf("Up key\n");
              break;            
            case SDLK_DOWN:
              printf("Down key\n");
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
