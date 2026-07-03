#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SCREEN_TITLE "Point"

struct Game {
  SDL_Renderer *renderer;
  SDL_Window *window;
};

void creation(struct Game *game1);
int main() {
  struct Game game1;
  game1.window = NULL;
  game1.renderer = NULL;
  creation(&game1);
  SDL_SetRenderDrawColor(game1.renderer, 0, 255, 255, 255);
  SDL_RenderClear(game1.renderer);
  SDL_RenderPresent(game1.renderer);
  SDL_Delay(5000);
  SDL_Quit();

  return EXIT_SUCCESS;
}

void creation(struct Game *game1) {
  SDL_Init(SDL_INIT_EVERYTHING);

  game1->window =
      SDL_CreateWindow(SCREEN_TITLE, SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  game1->renderer = SDL_CreateRenderer(game1->window, -1, 0);
}
