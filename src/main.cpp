#include <iostream>

#include "./sdl/sdl.hpp"

auto print = [](auto &&... args) -> void { (std::cout << ... << args) << '\n'; };

auto main() -> int
{
  sdl::create_window(800, 600);

  while (true)
  {
  }
}