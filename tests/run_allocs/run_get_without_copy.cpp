#include "dict.hpp"

#include <string>
#include <cassert>
#include <stdlib.h>

using namespace utils;

namespace
{
    bool EnableAllocs_s{ true };
}

void* operator new(std::size_t size)
{
    if (!EnableAllocs_s)
    {
        std::bad_alloc exception;
        throw exception;
    }
    void * p = malloc(size);
    return p;
}


void operator delete(void* p) noexcept
{
    free(p);
}


int main() {
  dict_t dict;
  std::string key{ "very_long_key_to_avoid_small_object_optimization_in_clang" };
  dict.put(key, 10);

  EnableAllocs_s = false;
  assert(dict.get<int>(key) == 10);
  (void)dict.get_ptr<int>(key);
  return 0;
}
