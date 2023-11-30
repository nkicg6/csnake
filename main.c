#include <stdio.h>
#include <stdlib.h>
#include "SDL2/SDL.h"

#define WINDOW_W 640
#define WINDOW_H 640
#define GAME_SIZE 600
#define GAME_X 20
#define GAME_Y 20
#define STEP_SIZE 20
#define MVMT_SPEED 50
#define REFRESH_RATE 1000/15
#define MAX_SNAKE_LEN 500
#define INIT_SIZE 15
#define BLOCK_SCORE 10

enum State {
  PAUSED,
  MOVE_LEFT,
  MOVE_RIGHT,
  MOVE_UP,
  MOVE_DOWN,
  CONTACT,
  EAT_FRUIT,
};

struct Game {
  int window_w; 
  int window_h;
  int game_w;
  int game_h;
  int step;
  int mvmt_speed;
  int max_len;
  int snake_len;
  int head_x;
  int head_y;
  int score;
  enum State state;
  SDL_Rect game_rect;
  SDL_Rect fruit;
  SDL_Window* window;
  SDL_Renderer* win_ren;
  SDL_Rect *body;
};

SDL_bool check_collision(struct Game *g, int next_x, int next_y){
  //bounds
  if ((next_x > g->game_rect.x+g->game_rect.w-g->step) || (next_x < g->game_rect.x) || (next_y < g->game_rect.y) || (next_y > g->game_rect.h+g->game_rect.y - g->step)) {

    g->state=CONTACT;
    return SDL_TRUE;
  } 
  SDL_Rect next_head = {.x=next_x, .y=next_y, .w=g->step, .h=g->step};
  for (int i = 1; i< g->snake_len-1;i++){
    if (SDL_HasIntersection(&next_head, &g->body[i])){
      g->state=CONTACT;
      return SDL_TRUE;
    }
  } 
  //food hit
  if (SDL_HasIntersection(&g->body[0], &g->fruit)){
    g->state = EAT_FRUIT;
  }
  return SDL_FALSE;
}

SDL_Rect make_fruit(struct Game *g){
  int x,y;
  SDL_bool col = SDL_TRUE;
  while(col){
    x = rand() % (g->game_rect.w + g->game_rect.x - g->step);
    y = rand() % (g->game_rect.h + g->game_rect.y - g->step);
    col = check_collision(g, x, y);
  }
  return (SDL_Rect){.x=x,.y=y,.w=g->step, .h=g->step};
}

void grow_snake(struct Game *g){
  g->score += BLOCK_SCORE;
  printf("Score = %d\n", g->score);
  printf("Snake len = %d\n", g->snake_len);
  if (g->snake_len < MAX_SNAKE_LEN){
    g->body[g->snake_len++] = g->fruit;
    g->fruit = make_fruit(g);
  }
}
void update_snake_pos(struct Game *g, enum State next_state){
  //check for hits, then update the position.
  int next_x, next_y; 
  switch (next_state) {
    case PAUSED:
      g->state=PAUSED;
      return;
      break;
    case MOVE_LEFT:
      next_x = g->head_x - g->step;
      next_y = g->head_y;
      if (check_collision(g, next_x, next_y)){return;}
      if (g->state == EAT_FRUIT){grow_snake(g);}
      g->state=MOVE_LEFT;
      g->head_x -=g->step;
      for(int i=g->snake_len-1;i>0;i--){
        g->body[i]=g->body[i-1];
      }
      g->body[0].x= g->head_x;
      break;
    case MOVE_RIGHT:
      next_x = g->head_x + g->step;
      next_y = g->head_y;
      if (check_collision(g, next_x, next_y)){return;}
      if (g->state == EAT_FRUIT){grow_snake(g);}
      g->head_x +=g->step;
      for(int i=g->snake_len-1;i>0;i--){
        g->body[i]=g->body[i-1];
      }
      g->body[0].x = g->head_x;
      g->state=MOVE_RIGHT;
      break;
    case MOVE_UP:
      next_x = g->head_x;
      next_y = g->head_y - g->step;
      if (check_collision(g, next_x, next_y)){return;}
      if (g->state == EAT_FRUIT){grow_snake(g);}
      g->head_y -= g->step;
      for(int i=g->snake_len-1;i>0;i--){
        g->body[i]=g->body[i-1];
      }
      g->body[0].y = g->head_y;
      g->state=MOVE_UP;
      break;
    case MOVE_DOWN:
      next_x = g->head_x;
      next_y = g->head_y + g->step;
      if (check_collision(g, next_x, next_y)){return;}
      if (g->state == EAT_FRUIT){grow_snake(g);}
      g->head_y += g->step;
      for(int i=g->snake_len-1;i>0;i--){
        g->body[i]=g->body[i-1];
      }
      g->body[0].y = g->head_y; 
      g->state=MOVE_DOWN;
      break;
    case CONTACT:
      return;
      break;
  }
}

void draw_game(struct Game *g){
  SDL_SetRenderDrawColor(g->win_ren, 255,255,255,255);
  SDL_RenderClear(g->win_ren);
  SDL_SetRenderDrawColor(g->win_ren, 200, 200, 200, 255);
  if (g->state == CONTACT){
    SDL_SetRenderDrawColor(g->win_ren, 255, 0, 0, 100);
    SDL_RenderFillRect(g->win_ren, &g->game_rect);
  } else {
    SDL_RenderDrawRect(g->win_ren, &g->game_rect);
  }
  SDL_SetRenderDrawColor(g->win_ren, 200, 0, 200, 255);
  SDL_RenderFillRect(g->win_ren, &g->body[0]);
  SDL_SetRenderDrawColor(g->win_ren, 200, 200, 200, 100);
  SDL_RenderFillRects(g->win_ren, &g->body[1], g->snake_len-1);
  SDL_SetRenderDrawColor(g->win_ren, 0, 255, 0, 255);
  SDL_RenderFillRect(g->win_ren, &g->fruit);
  SDL_RenderPresent(g->win_ren);
}


struct Game init(void){
  SDL_Rect* body = malloc(MAX_SNAKE_LEN * sizeof(SDL_Rect));
  if (body == NULL){
    puts("Failed to allocate snake");
    exit(EXIT_FAILURE);
  }
  body[0] = (SDL_Rect){.x=GAME_SIZE/2, .y=GAME_SIZE/4, .w=STEP_SIZE, .h=STEP_SIZE};
  for (int i=1;i<INIT_SIZE-1;i++){
    body[i] = (SDL_Rect){.x=(GAME_SIZE/2)-i*STEP_SIZE, .y=GAME_SIZE/4, .w=STEP_SIZE, .h=STEP_SIZE};
  }

  struct Game game = {.window_w=WINDOW_W, 
    .window_h=WINDOW_H, 
    .game_w=GAME_SIZE, 
    .game_h=GAME_SIZE, 
    .step=STEP_SIZE,
    .game_rect=(SDL_Rect){.x=GAME_X, .y=GAME_Y, .h=GAME_SIZE, .w=GAME_SIZE},
    .state=PAUSED,
    .score=0,
    .mvmt_speed=MVMT_SPEED,
    .max_len=MAX_SNAKE_LEN,
    .snake_len=INIT_SIZE,
    .head_x=GAME_SIZE/2,
    .head_y=GAME_SIZE/4,
    .body=body,
  };
  SDL_CreateWindowAndRenderer(WINDOW_W, WINDOW_H, SDL_WINDOW_RESIZABLE, &game.window, &game.win_ren);
  if (!game.window || !game.win_ren) {
    printf("Failed to create window or renderer. Error: %s", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  SDL_SetWindowTitle(game.window, "snake");
  game.fruit = make_fruit(&game);
  return game;
}


int main(void){
  if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_EVENTS)!=0){
    puts("Failed to init SDL");
    exit(EXIT_FAILURE);
  }

  struct Game game = init();
  SDL_bool quit = SDL_FALSE;
  SDL_Event event;
  int next_move_time = SDL_GetTicks() + game.mvmt_speed;
  enum State next_state = PAUSED;


  while(!quit){
    int last_tick = SDL_GetTicks();
    while (SDL_PollEvent(&event)){
      if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_q){
        printf("Quit event\n");
        quit = SDL_TRUE;
        break;
      } else {
        if (event.type == SDL_KEYDOWN){
          switch (event.key.keysym.sym){
            case SDLK_SPACE:
              puts("Paused, press an arrow key to continue");
              next_state = PAUSED;
              break;
            case SDLK_i:
              puts("DEBUG:");
              printf("Head x,y: %d, %d, \n", game.head_x, game.head_y); 
              break;
            case SDLK_RIGHT:
              next_state = MOVE_RIGHT;
              break;
            case SDLK_LEFT:
              next_state=MOVE_LEFT;
              break;
            case SDLK_UP:
              next_state=MOVE_UP;
              break;
            case SDLK_DOWN:
              next_state=MOVE_DOWN;
              break;
            case SDLK_r:
              if (game.state == CONTACT){
                game = init();
                game.state = PAUSED;
                next_state = PAUSED;
              } 
              break;
            default:
              break;
          }
        }
      }
    }

    if ( SDL_GetTicks() >= next_move_time) {
      //update
      int cur_delay = SDL_GetTicks() - last_tick;
      if (cur_delay < REFRESH_RATE) {
        SDL_Delay(cur_delay);
      }
      update_snake_pos(&game, next_state);
      //render
      draw_game(&game);
      next_move_time = SDL_GetTicks() + game.mvmt_speed;
    }
  }
  SDL_DestroyRenderer(game.win_ren);
  SDL_DestroyWindow(game.window);
  exit(0);
}
