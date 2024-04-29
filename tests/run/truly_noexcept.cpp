#include "dict.hpp"

#include <cassert>
#include <stdexcept>

using namespace utils;
using namespace std;

struct BrokenType {
  BrokenType() = default;
  BrokenType(BrokenType &&) = default;
  BrokenType(const BrokenType &) {
    throw runtime_error("BrokenType(const BrokenType&)");
  }

  BrokenType &operator=(BrokenType &&) = default;
  BrokenType &operator=(const BrokenType &) {
    throw runtime_error("BrokenType &operator=(const BrokenType &)");
  }
};

bool operator==(const BrokenType &, const BrokenType &) { return true; }

int main() {
  dict_t dict;
  BrokenType value;

  try {
    dict.put("key", value);
  } catch (const runtime_error &) {
  }

  assert(dict.empty());
  return 0;
}
