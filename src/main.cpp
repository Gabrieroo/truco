#include <iostream>

auto print = [](auto &&... args) -> void { (std::cout << ... << args) << '\n'; };

auto main() -> int
{
    print("hello ", "world", "!");
}