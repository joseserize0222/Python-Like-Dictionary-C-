#include "dict.hpp"

using namespace utils;

int main() {
  dict_t dict = {{"a", 42}};

  try {
    dict.get<const int>("a");
  } catch (const invalid_type_exception &) {
  }

  try {
    dict.get<int>("aa");
  } catch (const no_key_exception &) {
  }

  return 0;
}