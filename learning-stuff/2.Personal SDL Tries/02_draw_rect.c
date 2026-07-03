#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main() {
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Window *pwindow =
      SDL_CreateWindow("My Window", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
  SDL_Renderer *prenderer = SDL_CreateRenderer(pwindow, -1, 0);
  SDL_RenderClear(prenderer);
  SDL_SetRenderDrawColor(prenderer, 255, 255, 255, 255);
  SDL_RenderPresent(prenderer);
  SDL_Delay(5000);

  return 0;
}
