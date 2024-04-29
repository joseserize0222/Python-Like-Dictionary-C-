#include "dict.hpp"

using namespace utils;

void foo(const dict_t& dict)
{
    *(dict.get_ptr<int>("0")) = 20;
}

int main() {
  dict_t dict;
  dict.put("0", 10);

  foo(dict);
  return 0;
}
