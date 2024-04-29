#include "dict.hpp"
#include "dict_serialization.hpp"
#include "dict_json.hpp"

#include <cassert>
#include <sstream>
#include <iostream>

using namespace utils;
using namespace std;

int main() {
  float fValue = 1.5f;

  unsigned short usValue = 2;
  short sValue = 3;

  int iValue = -7;
  unsigned int uiValue = 8;

  unsigned long ulValue = 1ul;
  long lValue = 2l;

  dict_t dict;

  dict.put("float", fValue);

  dict.put("usValue", usValue);
  dict.put("sValue", sValue);

  dict.put("iValue", iValue);
  dict.put("uiValue", uiValue);

  dict.put("ulValue", ulValue);
  dict.put("lValue", lValue);

  stringstream ss;
  save_to_json(ss, dict);

  dict_t loaded_dict;
  load_from_json(ss, loaded_dict);

  const double d = loaded_dict.get<double>("float");
  assert(d > 1.49 && d < 1.51);

  assert(loaded_dict.get<int>("usValue") == 2);
  assert(loaded_dict.get<int>("sValue") == 3);

  assert(loaded_dict.get<int>("iValue") == -7);
  assert(loaded_dict.get<int>("uiValue") == 8);

  assert(loaded_dict.get<int>("ulValue") == 1);
  assert(loaded_dict.get<int>("lValue") == 2);

  return 0;
}
