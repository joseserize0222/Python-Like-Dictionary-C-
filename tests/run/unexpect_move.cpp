#include "dict.hpp"

#include <cassert>

using namespace utils;
using namespace std;

int main() {
  string value = "value";

  dict_t dict;

  dict.put("key", value);

  assert(!value.empty());

  return 0;
}