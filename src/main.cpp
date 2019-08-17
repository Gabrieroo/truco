#include <iostream>

#include "./core/sdl/SDL.h"
#include "./core/operators/Pipe.h"

auto print = [](auto &&... args) -> void { (std::cout << ... << args) << '\n'; };

auto main() -> int
{
  sdl::createWindow(1280, 720);

  auto texture = sdl::loadTexture("assets/background.jpg");

  auto card = sdl::loadTexture("assets/sword-1.jpg");

  while (true)
  {
    sdl::startFrame();

    sdl::events();

    sdl::texture(texture);
    sdl::texture(card);

    sdl::endFrame();
  }
}
