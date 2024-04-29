#include "dict.hpp"

using namespace utils;

int main() {
  const dict_t c_dict;
  dict_t dict;

  static_assert(std::is_same_v<decltype(c_dict.get<int>("key")), const int &>);
  static_assert(std::is_same_v<decltype(c_dict.get_ptr<int>("key")), const int *>);

  static_assert(std::is_same_v<decltype(dict.get<int>("key")), int &>);
  static_assert(std::is_same_v<decltype(dict.get_ptr<int>("key")), int *>);

  return 0;
}
