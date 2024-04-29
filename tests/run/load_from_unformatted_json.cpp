#include "dict.hpp"
#include "dict_json.hpp"

#include <cassert>
#include <sstream>

using namespace utils;

int main() {
  const std::string json = R"({  "  a"  :   " 1 "   })";
  const dict_t origin = {{"  a", std::string(" 1 ")}};

  std::stringstream ss(json);

  dict_t dict;
  load_from_json(ss, dict);

  assert(dict == origin);

  return 0;
}
