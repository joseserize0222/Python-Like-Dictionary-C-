#include "dict.hpp"

using namespace utils;
using namespace std;

void foo(const dict_t& dict) {
    (void)dict.get<string>("key");
}


void bar(const dict_t& dict) {
    (void)dict.get_ptr<string>("key");
}


void baz(const dict_t& dict) {
    (void)dict.empty();
    (void)dict.contains("key");
    (void)dict.is_dict("key");
}


int main() {
  const string value = "value";

  dict_t dict;

  dict.put("key", value);
  
  foo(dict);
  bar(dict);
  baz(dict);

  return 0;
}


