#include "dict.hpp"
#include "dict_json.hpp"

#include <sstream>
#include <iostream>

using namespace utils;

int main() {
  const dict_t dict;
  std::stringstream ss;
  save_to_json(ss, dict);
  std::cout << ss.str() << std::endl;
  return 0;
}
