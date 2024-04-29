#include "dict.hpp"

using namespace utils;

struct foo {};

bool operator==(const foo &, const foo &) { return true; }

const foo make_const_foo() { return {}; }

int main() {
  dict_t dict;
  dict.put("key", make_const_foo());

  dict.get<foo>("key") = {};

  return 0;
}
