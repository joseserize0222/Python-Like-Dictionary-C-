#include "dict.hpp"

namespace
{
    struct unmovable
    {
        unmovable() = default;
        unmovable(const unmovable& other) = default;
        unmovable& operator=(const unmovable& other) = default;

        unmovable(unmovable&& other) = delete;
        unmovable& operator=(unmovable&& other) = delete;
    };

    bool operator==(const unmovable&, const unmovable&)
    {
        return true;
    }
}

int main() {
  utils::dict_t dict;
  dict.put("key", unmovable());
  return 0;
}

