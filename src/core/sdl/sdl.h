#pragma once

#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <utility>
#include <cstdlib>
#include <memory>

namespace sdl
{

namespace detail
{
static constexpr Uint8 FPS = 60;
static constexpr Uint8 frameDelay = 1000 / FPS;

static SDL_Window *window;
static SDL_Renderer *renderer;

static Uint32 frameTime;
static Uint32 frameStart;

static unsigned int _WINDOW_WIDTH;
static unsigned int _WINDOW_HEIGHT;

static SDL_Event event_handler;
} // namespace detail

static unsigned int const &WINDOW_WIDTH = detail::_WINDOW_WIDTH;
static unsigned int const &WINDOW_HEIGHT = detail::_WINDOW_HEIGHT;

struct Texture
{
  SDL_Texture *texture;
  SDL_Rect source;

  Texture(SDL_Texture *t) : texture(t)
  {
    SDL_QueryTexture(texture, nullptr, nullptr, &source.w, &source.h);
    source.x = 0;
    source.y = 0;
  }
};

auto createWindow(int width, int height, bool fullscreen = false) -> void
{
  detail::_WINDOW_WIDTH = width;
  detail::_WINDOW_HEIGHT = height;

  int flags = fullscreen ? SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN : SDL_WINDOW_OPENGL;

  SDL_Init(SDL_INIT_EVERYTHING);

  IMG_Init(IMG_INIT_JPG |
           IMG_INIT_PNG |
           IMG_INIT_TIF);

  SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, flags, &detail::window, &detail::renderer);
  SDL_SetRenderDrawColor(detail::renderer, 0xff, 0xb4, 0xff, 0xff);
}

auto loadTexture(const char *file) -> std::shared_ptr<Texture>
{
  SDL_Surface *tempImage = IMG_Load(file);
  SDL_Surface *optimizedImage = SDL_ConvertSurface(tempImage, tempImage->format, 0);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(detail::renderer, optimizedImage);

  SDL_FreeSurface(tempImage);
  SDL_FreeSurface(optimizedImage);

  return std::shared_ptr<Texture>(new Texture(texture), [](Texture *t) -> void { SDL_DestroyTexture(t->texture); });
}

auto setColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 0) -> void
{
  SDL_SetRenderDrawColor(detail::renderer, r, g, b, a);
}

auto get_mouse_position() -> std::pair<int, int>
{
  int x_pos;
  int y_pos;

  SDL_GetMouseState(&x_pos, &y_pos);
  return std::make_pair(x_pos, y_pos);
}

auto rectangle(float x, float y, int width, int height) -> void
{
  SDL_Rect rectangle;
  rectangle.x = x;
  rectangle.y = y;
  rectangle.w = width;
  rectangle.h = height;

  SDL_RenderFillRect(detail::renderer, &rectangle);
}

auto point(int x, int y) -> void
{
  SDL_RenderDrawPoint(detail::renderer, x, y);
}

auto line(int x1, int y1, int x2, int y2) -> void
{
  SDL_RenderDrawLine(detail::renderer, x1, y1, x2, y2);
}

auto paintScreen(Uint8 r, Uint8 g, Uint8 b) -> void
{
  SDL_RenderClear(detail::renderer);
  setColor(r, g, b);
  rectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}

auto update() -> void
{
  SDL_RenderPresent(detail::renderer);
}

auto startFrame() -> void
{
  SDL_PumpEvents();
  detail::frameStart = SDL_GetTicks();
}

auto endFrame() -> void
{
  update();
  detail::frameTime = SDL_GetTicks() - detail::frameStart;
  if (detail::frameDelay > detail::frameTime)
  {
    SDL_Delay(detail::frameDelay - detail::frameTime);
  }
}

auto texture(std::shared_ptr<Texture> const &texture, SDL_Rect *dest = nullptr,
             double angle = 0.0, SDL_RendererFlip &&flip = SDL_FLIP_NONE) -> void
{
  SDL_RenderCopyEx(detail::renderer,
                   texture->texture,
                   &texture->source,
                   dest,
                   angle,
                   nullptr,
                   std::forward<decltype(flip)>(flip));
}

auto sleep(size_t ms) -> void
{
  SDL_Delay(ms);
}

auto quit() -> void
{
  SDL_DestroyRenderer(detail::renderer);
  SDL_DestroyWindow(detail::window);
  SDL_Quit();
  std::exit(EXIT_SUCCESS);
}

auto events() -> void
{
  SDL_PollEvent(&detail::event_handler);

  Uint8 *keysArray = const_cast<Uint8 *>(SDL_GetKeyboardState(nullptr));

  if (detail::event_handler.type == SDL_QUIT || keysArray[SDL_SCANCODE_ESCAPE])
  {
    quit();
  }
}
} // namespace sdl
