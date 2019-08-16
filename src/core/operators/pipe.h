#pragma once

template <typename InitialValue, typename Fn>
constexpr auto operator|(InitialValue &&arg, Fn const &fn) -> InitialValue
{
  return fn(std::forward<InitialValue>(arg));
}