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

  SDL_Window *window;
  window = SDL_CreateWindow("Snake", 0,0,640,480, SDL_WINDOW_OPENGL);
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
