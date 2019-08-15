#pragma once

#define SDL_MAIN_HANDLED

#ifdef WIN32
#include "../../include/windows/sdl2/SDL.h"
#else
#include "../../include/linux/sdl2/SDL.h"
#endif

#include <utility>
#include <cstdlib>

namespace sdl
{
namespace
{
static constexpr unsigned int FPS = 60;
static constexpr unsigned int frame_delay = 1000 / FPS;

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
auto create_window(int width, int height, bool fullscreen = false) -> void
{
  WINDOW_WIDTH = width;
  WINDOW_HEIGHT = height;
  WINDOW_HALF_WIDTH = WINDOW_WIDTH / 2;
  WINDOW_HALF_HEIGHT = WINDOW_HEIGHT / 2;

  int flags = fullscreen ? SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN : SDL_WINDOW_OPENGL;

  SDL_Init(SDL_INIT_EVERYTHING);

  SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, flags, &window, &renderer);
  SDL_SetRenderDrawColor(renderer, 0xff, 0xb4, 0xff, 0xff);
}

auto sleep(size_t ms) -> void
{
  SDL_Delay(ms);
}

auto quit() -> void
{
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  std::exit(EXIT_SUCCESS);
}

auto handle_events() -> void
{
  SDL_PollEvent(&event_handler);

  Uint8 *keysArray = const_cast<Uint8 *>(SDL_GetKeyboardState(nullptr));

  if (event_handler.type == SDL_QUIT || keysArray[SDL_SCANCODE_ESCAPE])
  {
    quit();
  }
}

auto set_color(int r, int g, int b, int a = 0) -> void
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

auto render_rectangle(float x, float y, int width, int height) -> void
{
  SDL_Rect rectangle;
  rectangle.x = x;
  rectangle.y = y;
  rectangle.w = width;
  rectangle.h = height;

  SDL_RenderFillRect(renderer, &rectangle);
}

auto render_point(int x, int y) -> void
{
  SDL_RenderDrawPoint(renderer, x, y);
}

auto render_line(int x1, int y1, int x2, int y2) -> void
{
  SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

auto paint_screen(int r = 0, int g = 0, int b = 0) -> void
{
  SDL_RenderClear(renderer);
  set_color(r, g, b);
  render_rectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}

auto update_screen() -> void
{
  SDL_RenderPresent(renderer);
}

auto start_frame() -> void
{
  SDL_PumpEvents();
  frame_start = SDL_GetTicks();
}

auto end_frame() -> void
{
  update_screen();
  frame_time = SDL_GetTicks() - frame_start;
  if (frame_delay > frame_time)
  {
    SDL_Delay(frame_delay - frame_time);
  }
}

} // namespace sdl