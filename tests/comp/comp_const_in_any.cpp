#include "dict.hpp"

using namespace utils;
using namespace std;

int main() {
  const string value = "value";

  dict_t dict;

  dict.put("key", value);

  dict.get<string>("key").append("_modified");

  return 0;
}

