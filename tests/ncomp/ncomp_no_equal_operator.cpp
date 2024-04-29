#include "dict.hpp"

using namespace utils;

struct Dummy {
  int dummy;
};

int main() {
  dict_t dict = {{"key", Dummy{}}};

  return 0;
}