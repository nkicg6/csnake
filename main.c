#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "SDL2/SDL.h"

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
  SDL_SetRenderDrawColor(renderer, 255,25,255,255);
  SDL_Rect rect;
  rect.x = 25;
  rect.y = 0;
  rect.w = 150;
  rect.h = 150;

  SDL_RenderFillRect(renderer, &rect);
  SDL_RenderDrawRect(renderer, &rect);
  SDL_RenderPresent(renderer);
  while(!quit){
    SDL_Delay(250);
    while (SDL_PollEvent(&event)){

      switch (event.type) {

        case SDL_KEYDOWN:
          if (event.key.keysym.sym == SDLK_q) {
            printf("Quit Key\n");
            quit = true;
            SDL_DestroyWindow(window);
          } else {
            printf("Unknown key pressed\n");
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
