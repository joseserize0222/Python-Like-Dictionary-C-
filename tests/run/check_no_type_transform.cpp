#include "dict.hpp"

#include <cassert>
#include <cstdint>
#include <limits>

using namespace utils;

template <typename T> void test() {
  constexpr T max_value = std::numeric_limits<T>::max();
  constexpr T min_value = std::numeric_limits<T>::min();

  dict_t dict = {{"key", max_value}, {"another_key", min_value}};
  assert(dict.get<T>("key") == max_value);
  assert(dict.get<T>("another_key") == min_value);
}

template <typename... Ts> void test_for() { (test<Ts>(), ...); }

int main() {
  test_for<std::int8_t, std::uint8_t, std::int16_t, std::uint16_t, std::int32_t, std::uint32_t, std::int64_t, std::uint64_t, bool, float, double>();
  return 0;
}
