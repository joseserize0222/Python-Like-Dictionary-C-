#include "dict.hpp"
#include "dict_serialization.hpp"

#include <map>
#include <cassert>
#include <string>

using namespace utils;

void do_read(const dict_t& dict, std::map<std::string, int>& v)
{
    read(dict, v);
}

void do_write(dict_t& dict, const std::map<std::string, int>& v)
{
    write(dict, v);
}

int main() {
  dict_t dict;

  dict.put("0", 10);
  dict.put("1", 11);
  
  std::map<std::string, int> m;
  
  do_read(dict, m);
  
  assert(m["0"] == 10);
  assert(m["1"] == 11);
  
  dict_t new_dict;
  
  do_write(new_dict, m);
  
  assert(new_dict.get<int>("0") == 10);
  assert(new_dict.get<int>("1") == 11);
  return 0;
}
