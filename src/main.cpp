#include <iostream>

#include "./core/sdl/sdl.h"
#include "./core/operators/pipe.h"

auto print = [](auto &&... args) -> void { (std::cout << ... << args) << '\n'; };

auto main() -> int
{
  sdl::createWindow(800, 600);

  auto texture = sdl::loadTexture("assets/background.jpg");

  while (true)
  {
    sdl::startFrame();

    sdl::paintScreen(0, 0, 0);

    sdl::events();

    sdl::texture(texture);

    sdl::endFrame();
  }
}
