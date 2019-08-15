#pragma once

#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <utility>
#include <cstdlib>

namespace sdl
{
namespace
{
static constexpr Uint8 FPS = 60;
static constexpr Uint8 frame_delay = 1000 / FPS;

static Uint32 frame_time;
static Uint32 frame_start;

static int WINDOW_WIDTH;
static int WINDOW_HEIGHT;
static int WINDOW_HALF_WIDTH;
static int WINDOW_HALF_HEIGHT;

static SDL_Event event_handler;

static SDL_Window *window;
static SDL_Renderer *renderer;

} // namespace
static auto createWindow(int width, int height, bool fullscreen = false) -> void
{
  WINDOW_WIDTH = width;
  WINDOW_HEIGHT = height;
  WINDOW_HALF_WIDTH = WINDOW_WIDTH / 2;
  WINDOW_HALF_HEIGHT = WINDOW_HEIGHT / 2;

  int flags = fullscreen ? SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN : SDL_WINDOW_OPENGL;

  SDL_Init(SDL_INIT_EVERYTHING);

  IMG_Init(IMG_INIT_JPG |
           IMG_INIT_PNG |
           IMG_INIT_TIF);

  SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, flags, &window, &renderer);
  SDL_SetRenderDrawColor(renderer, 0xff, 0xb4, 0xff, 0xff);
}

static auto loadTexture(const char *file) -> SDL_Texture *
{
  SDL_Surface *tempImage = IMG_Load(file);
  SDL_Surface *optimizedImage = SDL_ConvertSurface(tempImage, tempImage->format, 0);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, optimizedImage);

  SDL_FreeSurface(tempImage);
  SDL_FreeSurface(optimizedImage);

  return texture;
}

static auto texture(SDL_Texture *texture, SDL_Rect *source, SDL_Rect *destination, double angle = 0.0, SDL_RendererFlip &&flip = SDL_FLIP_NONE) -> void
{
  SDL_RenderCopyEx(renderer, texture, source, destination, angle, nullptr, std::forward<decltype(flip)>(flip));
}

static auto sleep(size_t ms) -> void
{
  SDL_Delay(ms);
}

static auto quit() -> void
{
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  std::exit(EXIT_SUCCESS);
}

static auto events() -> void
{
  SDL_PollEvent(&event_handler);

  Uint8 *keysArray = const_cast<Uint8 *>(SDL_GetKeyboardState(nullptr));

  if (event_handler.type == SDL_QUIT || keysArray[SDL_SCANCODE_ESCAPE])
  {
    quit();
  }
}

static auto setColor(int r, int g, int b, int a = 0) -> void
{
  SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

std::pair<int, int> get_mouse_position()
{
  int x_pos;
  int y_pos;

  SDL_GetMouseState(&x_pos, &y_pos);
  return std::make_pair(x_pos, y_pos);
}

static auto rectangle(float x, float y, int width, int height) -> void
{
  SDL_Rect rectangle;
  rectangle.x = x;
  rectangle.y = y;
  rectangle.w = width;
  rectangle.h = height;

  SDL_RenderFillRect(renderer, &rectangle);
}

static auto point(int x, int y) -> void
{
  SDL_RenderDrawPoint(renderer, x, y);
}

static auto line(int x1, int y1, int x2, int y2) -> void
{
  SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

static auto paintScreen(int r = 0, int g = 0, int b = 0) -> void
{
  SDL_RenderClear(renderer);
  setColor(r, g, b);
  rectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}

static auto update() -> void
{
  SDL_RenderPresent(renderer);
}

static auto startFrame() -> void
{
  SDL_PumpEvents();
  frame_start = SDL_GetTicks();
}

static auto endFrame() -> void
{
  update();
  frame_time = SDL_GetTicks() - frame_start;
  if (frame_delay > frame_time)
  {
    SDL_Delay(frame_delay - frame_time);
  }
}

} // namespace sdl