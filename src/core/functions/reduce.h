#pragma once

#include <type_traits>

template <typename T, typename Fn>
constexpr auto reduce(T const &array, Fn const &fn) -> T
{
  auto accumulator = array[0];

  for (size_t i = 1; i < std::extent(array); ++i)
  {
    accumulator = fn(accumulator, array[i], i);
  }

  return accumulator;
}