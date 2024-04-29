#include "dict.hpp"

#include <cassert>

using namespace utils;
using namespace std;

struct CopyCounter {
  CopyCounter() = default;
  CopyCounter(const CopyCounter &) noexcept { counter++; }
  CopyCounter(CopyCounter &&) = default;

  CopyCounter &operator=(CopyCounter &&) = default;
  CopyCounter &operator=(const CopyCounter &) noexcept {
    counter++;
    return *this;
  }

  inline static size_t counter = 0;
};

bool operator==(const CopyCounter &, const CopyCounter &) { return true; }

int main() {
  auto copiesBefore = CopyCounter::counter;

  dict_t dict;

  dict.put("key", CopyCounter());

  auto copiesAfter = CopyCounter::counter;
  assert(copiesBefore == copiesAfter);

  return 0;
}