#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

int main(void) {
  int init = SDL_Init(SDL_INIT_EVENTS|SDL_INIT_VIDEO);
  if (0!=init){
    printf("Failed to init, exit code: %d. Exiting\n", init);
    exit(EXIT_FAILURE);
  }else{
    printf("Initialized SDL2\n");
  }
  SDL_Event event;
  bool quit = false;

  while(!quit){
    SDL_CreateWindow("Snake", 0,0,640,480, SDL_WINDOW_OPENGL);
    SDL_Delay(500);
    while (SDL_PollEvent(&event)){
      switch (event.type) {
        case SDL_KEYDOWN:
          printf("Key pressed\n");
          switch (event.key.keysym.sym) {
            case SDLK_q:
              printf("Quit detected\n");
              quit = true;
            default:
              break;
          }
          break;
      }

    }
  }
}
