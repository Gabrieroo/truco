#pragma once

#include <type_traits>

template <typename T, typename Fn>
constexpr auto map(T const &array, Fn const &fn) -> T
{
  T result;

  for (size_t i = 0; i < std::extent(array); ++i)
  {
    result[i] = fn(array[i], i);
  }

  return result;
}