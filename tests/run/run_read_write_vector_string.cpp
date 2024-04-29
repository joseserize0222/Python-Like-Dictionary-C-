#include "dict.hpp"
#include "dict_serialization.hpp"

#include <vector>
#include <cassert>
#include <string>

using namespace utils;
using namespace std;

int main() {
  dict_t dict;
  const std::string first_string("first_string");
  const std::string second_string("second_string");

  dict.put("0", first_string);
  dict.put("1", second_string);
  
  std::vector<std::string> v;
  read(dict, v);

  assert(v[0] == first_string);
  assert(v[1] == second_string);
  
  dict_t new_dict;
  write(new_dict, v);
  
  assert(new_dict.get<std::string>("0") == first_string);
  assert(new_dict.get<std::string>("1") == second_string);
  return 0;
}


