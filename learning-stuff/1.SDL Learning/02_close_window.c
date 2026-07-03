#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_quit.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define WINDOW_NAME "Window Name"
#define WINDTH 800
#define HEIGHT 600

struct Game {
  SDL_Window *window;
  SDL_Renderer *renderer;
};

bool init_game(struct Game *game);
void game_cleanup(struct Game *game, int exit_status) {
  SDL_DestroyRenderer(game->renderer);
  SDL_DestroyWindow(game->window);
  SDL_Quit();
  exit(exit_status);
}
int main(int argc, char *argv[]) {
  // Create game instance
  struct Game game1;
  game1.window = NULL;
  game1.renderer = NULL;
  if (init_game(&game1)) {
    game_cleanup(&game1, EXIT_FAILURE);
  }
  while (true) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        game_cleanup(&game1, EXIT_SUCCESS);
        break;
      case SDL_KEYDOWN:
        switch (event.key.keysym.scancode) {
        case SDL_SCANCODE_ESCAPE:
          game_cleanup(&game1, EXIT_SUCCESS);
          break;
        default:
          break;
        }
      default:
        break;
      }
    }
    SDL_SetRenderDrawColor(game1.renderer, 255, 0, 0, 255);
    SDL_RenderClear(game1.renderer);
    SDL_RenderPresent(game1.renderer);
    SDL_Delay(16);
  }
  game_cleanup(&game1, EXIT_SUCCESS);
  return 0;
}

bool init_game(struct Game *game) {
  if (SDL_Init(SDL_INIT_EVERYTHING)) {
    fprintf(stderr, "An Error has occured during initialization.%s",
            SDL_GetError());
    return true;
  }

  game->window = SDL_CreateWindow(WINDOW_NAME, SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED, WINDTH, HEIGHT, 0);

  if (!game->window) {
    fprintf(stderr, "An Error has occured during window creation.");
    return true;
  }
  game->renderer = SDL_CreateRenderer(game->window, -1, 0);
  if (!game->renderer) {
    fprintf(stderr, "Error during window creation.");
    return true;
  }
  return false;
}
