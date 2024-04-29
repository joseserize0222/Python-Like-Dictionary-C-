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
  constexpr size_t N = 1024;

  dict_t dict;

  for (size_t i = 0; i < N; i++) {
    if (i == N / 2) {
      dict.put(to_string(i), BrokenType());
    } else {
      dict.put(to_string(i), to_string(i));
    }
  }

  dict_t copy;

  try {
    copy = dict;
  } catch (const runtime_error &) {
  }

  assert(copy.empty());

  for (size_t i = 0; i < N; i++) {
    assert(!copy.contains(to_string(i)));
  }

  copy = std::move(dict);

  assert(!copy.empty());

  for (size_t i = 0; i < N; i++) {
    assert(copy.contains(to_string(i)));
  }

  return 0;
}
