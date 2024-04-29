#include "dict.hpp"
#include <cassert>

using namespace utils;

int main() {
  dict_t dict;

  dict.put("0", 10);
  dict.put("1", 11);

  dict.clear();

  assert(dict.empty());
  assert(!dict.contains("0"));
  assert(!dict.contains("1"));
  return 0;
}
