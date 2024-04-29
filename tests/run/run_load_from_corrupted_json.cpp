#include "dict.hpp"
#include "dict_json.hpp"

#include <cassert>
#include <sstream>

int main() {
  const std::string json = R"({  "  a  :   " 1 "   })";
  std::stringstream ss(json);

  utils::dict_t dict;
  bool const load_result = load_from_json(ss, dict);

  assert(!load_result);

  return 0;
}

