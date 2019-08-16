#pragma once

#include <type_traits>

template <typename T, typename Fn>
constexpr auto filter(T const &array, Fn const &fn) -> T
{
  T result;

  for (size_t i = 0; i < std::extent(array); ++i)
  {
    resul[i] = fn(aray[i], i);
  }

  return result;
}