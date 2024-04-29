#include "dict.hpp"
#include "dict_serialization.hpp"

#include <cassert>
#include <cstdint>
#include <limits>
#include <map>
#include <string>

template <typename T>
void test() {
  constexpr T max_value = std::numeric_limits<T>::max();
  constexpr T min_value = std::numeric_limits<T>::min();

  utils::dict_t dict;
  dict.put("key", min_value);
  dict.put("another_key", max_value);

  std::map<std::string, T> m;
  read(dict, m);

  assert(m["key"] == min_value);
  assert(m["another_key"] == max_value);

  utils::dict_t new_dict;
  write(new_dict, m);

  assert(new_dict.get<T>("key") == min_value);
  assert(new_dict.get<T>("another_key") == max_value);

}

template <typename... Ts>
void test_for()
{
  (test<Ts>(), ...);
}

int main() {
  test_for<char, std::int8_t, std::uint8_t, std::int16_t, std::uint16_t, std::int32_t, std::uint32_t, std::int64_t, std::uint64_t, bool, float, double>();
  return 0;
}

