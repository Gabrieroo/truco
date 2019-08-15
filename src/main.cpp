#include <iostream>

#include "./sdl/sdl.hpp"

auto print = [](auto &&... args) -> void { (std::cout << ... << args) << '\n'; };

auto main() -> int
{
  sdl::createWindow(800, 600);

  while (true)
  {
    sdl::startFrame();

    sdl::paintScreen();

    sdl::events();

    sdl::endFrame();
  }
}
