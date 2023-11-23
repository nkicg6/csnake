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
  OK,
  BOUNDARY,
} SnakeMvmtDirection;

void draw_game(SDL_Renderer *rend, SDL_Rect *rect, int r, int g, int b){
  SDL_SetRenderDrawColor(rend, r, g, b, 255);
  SDL_RenderDrawRect(rend, rect);
}

//TODO
void new_fruit(SDL_Renderer *rend, SDL_Rect *rect) {

}

const int TARGET_TICK = 1000/30;
const int MVMT_SPEED_MS = 50; // rate of movement update. Lower is faster
const int WIN_WIDTH = 640;
const int WIN_HEIGHT = 640; 
const int GAME_WIDTH = 500;
const int GAME_HEIGHT = 500;
const int GAME_POS_X = 20;
const int GAME_POS_Y = 20;
const int STEP_SIZE = 10;

const int GRID_N = ((GAME_WIDTH+GAME_POS_X)/STEP_SIZE)-1;

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

  if(SDL_SetRenderDrawBlendMode(win_renderer, SDL_BLENDMODE_BLEND)!=0){
    printf("Failed to set blend mode, SDL Error: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  SDL_Rect game_rect = {.x = GAME_POS_X, .y = GAME_POS_Y, .w = GAME_WIDTH, .h = GAME_HEIGHT}; 

  int head_start_pos_x = GAME_WIDTH/2;
  int head_start_pos_y = GAME_HEIGHT/2;

  // snake, fixed length for now.
  int tailLen = 19;
  SDL_Rect snake[tailLen];
  SDL_Rect head = {.x = head_start_pos_x, .y = head_start_pos_y, .w = STEP_SIZE, .h = STEP_SIZE};
  snake[0] = head;
  for (int i = 1; i < tailLen; i++){
    snake[i] = (SDL_Rect){.x = head_start_pos_x-STEP_SIZE*i, .y=head_start_pos_y, .w=STEP_SIZE,.h=STEP_SIZE};
  }

  SDL_Rect grid[GRID_N];

  for (int i=0,x=GAME_POS_X; i < GRID_N-1; i++){
    grid[i] = (SDL_Rect){.x=x, .y=GAME_POS_Y, .w=STEP_SIZE, .h=STEP_SIZE};
    x= x+STEP_SIZE;
  }

  SnakeMvmtDirection snake_direction = STOPPED;
  SnakeMvmtDirection state = OK;
  bool quit = false;
  int next_move_time = SDL_GetTicks() + MVMT_SPEED_MS;

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
            //debug info
            printf("headx = %d, headx right: %d, heady = %d, heady max = %d\n", snake[0].x, snake[0].x + snake[0].w, snake[0].y, snake[0].y + snake[0].h);
            break;

          case SDLK_LEFT:
            if (snake_direction == RIGHT || (snake_direction == STOPPED && state == BOUNDARY)) {
              printf("Can't turn like that!\n");
              break;
            }
            snake_direction = LEFT;
            printf("left button\n");
            break;

          case SDLK_RIGHT:
            if (snake_direction == LEFT || (snake_direction == STOPPED && state == BOUNDARY)) {
              printf("Can't turn like that!\n");
              break;
            }
            snake_direction = RIGHT;
            printf("right button\n");
            break;

          case SDLK_UP:
            if (snake_direction == DOWN || (snake_direction == STOPPED && state == BOUNDARY)) {
              printf("Can't turn like that!\n");
              break;
            }
            snake_direction = UP;
            printf("up button\n");
            break;

          case SDLK_DOWN:
            if (snake_direction == UP || (snake_direction == STOPPED && state == BOUNDARY)) {
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
    }
    // update events
    if ( SDL_GetTicks() >= next_move_time) {
      switch(snake_direction){
        case UP:
          if (snake[0].y-STEP_SIZE < game_rect.y) {
            printf("Exceeds bounds\n");
            snake_direction = STOPPED;
            break;
          }

          for (int i=tailLen-1; i>0; i--){
            snake[i] = snake[i-1];
          }

          snake[0].y -= STEP_SIZE;
          break;

        case DOWN:
          if (snake[0].y+snake[0].h+STEP_SIZE > game_rect.y+game_rect.h){
            snake_direction = STOPPED;
            break;
          }

          for (int i=tailLen-1; i>0; i--){
            snake[i] = snake[i-1];
          }
          snake[0].y += STEP_SIZE;
          break;

        case LEFT:
          if (snake[0].x-STEP_SIZE < game_rect.x){
            snake_direction = STOPPED;
            break;
          }

          for (int i=tailLen-1; i>0; i--){
            snake[i] = snake[i-1];
          }

          snake[0].x -= STEP_SIZE;
          break;

        case RIGHT:
          if (snake[0].x+snake[0].w+STEP_SIZE> game_rect.x+game_rect.w){
            printf("Exceeds bounds\n");
            snake_direction = STOPPED;
            break;
          }

          for (int i=tailLen-1; i>0; i--){
            snake[i] = snake[i-1];
          }

          snake[0].x += STEP_SIZE;
          break;
        case OK:
          break;
        case BOUNDARY:
        case STOPPED:
          break;
      }
      //render
      int cur_delay = SDL_GetTicks() - last_tick;
      if (cur_delay < TARGET_TICK) {
        SDL_Delay(cur_delay);
      }
      //fn to draw based on game struct with all shapes
      SDL_SetRenderDrawColor(win_renderer, 255,255,255,255);
      SDL_RenderClear(win_renderer);
      SDL_SetRenderDrawColor(win_renderer, 0,255,0,255);
      SDL_RenderFillRect(win_renderer, &snake[0]);
      SDL_SetRenderDrawColor(win_renderer, 255,25,255,100);
      SDL_RenderFillRects(win_renderer, &snake[1], tailLen-1); 
      SDL_SetRenderDrawColor(win_renderer, 200,200,200,100);
      SDL_RenderDrawRects(win_renderer, grid, GRID_N);
      draw_game(win_renderer, &game_rect, 200, 200, 200);
      SDL_RenderPresent(win_renderer);
      next_move_time = SDL_GetTicks() + MVMT_SPEED_MS;

      //uncomment for single step movemnet for debug
      //snake_direction = STOPPED;
    }
  }

  //cleanup
  SDL_DestroyRenderer(win_renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  exit(0);
}
