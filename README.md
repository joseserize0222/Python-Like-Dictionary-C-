# Dictionary

Create a dynamic dictionary `utils::dict_t` with the keys of a string type (`std::string`).
Such a dictionary allows you to put any instance of a copyable or movable type as a value, including the dictionary itself.
Keys are unique.

A dynamic dictionary is helpful for storing a hierarchical structure, for example, a configuration.
Also, such a dictionary can be used as an intermediate data structure for serializing C++ classes into various formats (json, xml, etc.), so as not to write serializers for each such format.

## Basic dictionary operations (file `dict.hpp`)


1. Add the value to the dict with the key:

```cpp
bool put(dict, key, value);
```

The operation saves the value category for the key and the value.
For example, if you pass a temporary instance it will not be copied, but will be moved (if supported).


2. Get an instance from a dict by key:

```cpp
<pointer on T>   get_ptr(dict, key);
<reference on T> get    (dict, key);
```

If there is no such key, or the type used in get does not match the type of the instance under this key, then the `get_ptr` function will return `nullptr`, a the `get` function will throw `utils::no_key_exception` or `utils::invalid_type_exception` accordingly.
These operations shouldn’t return a reference or a pointer to a non-const value if the stored value is const.

3. Deleting a key (returns true if and only if the element with key was present in the dict), clearing the entire dictionary, checking if the key is present, and checking if the dictionary is empty:

```cpp
bool remove  (dict, key);
bool contains(dict, key);
bool empty         (dict);
void clear         (dict);
```

4. Checking whether the entry using the current key is a dictionary

```cpp
bool is_dict (dict, key);
```

5. The dictionary can be compared for equality/inequality.

## Additional dictionary requirements


1. A dictionary allows you to create an instance through an initialization list:

```cpp
utils::dict_t dict =
{
        {"five", 5},
        {"almost_zero", 0.01},
        {"inner_dict",
                {
                        {"str_key", string("dict")},
                        {"pi", 3.14}
                }
        }
};
```

2. All dictionary operations provide a strong exception safety guarantee.


3. The dictionary can be copied and moved.


## Serialization (file `dict_serialization.hpp`)

As mentioned above, such a dynamic dictionary is convenient for serializing C++ classes.
Especially, if there is a reflection mechanism provided for your classes.
However, this reflection is not in the scope of this homework.

We only ask you to provide serialization for the objects that are a composition of a `vector<?>`, `map<string, ?>`, arithmetic types and strings.
We guarantee that the top-level instance is one of the containers (`vector`, `map`).
The vector should be serialized as a dictionary with keys equal to the index of the element (“0”, “1”, etc.).

It is important that the serialized instance is parsed hierarchically down to arithmetic types and strings.
Thus if a map of maps is serialized, then the value in the dictionary will also be a dictionary, not a map.


Required functions for the serialization:

1. Dictionary entries:

```cpp
void write(dict, value)
```

2. Readings from the dictionary:

```cpp
void read(dict, value)
```

Example:

```cpp
using composition_t = map<string, vector<map<string, int>>>;
composition_t origin_value =
{
        {
                "one",
                {
                        {},
                        {
                                {"single", 3}
                        },
                        {
                                {"double_1", 1}, {"double_2", 2}
                        }
                }
        },
        {
                "empty",
                {}
        }
};

dict_t dict;
write(dict, origin_value);

composition_t read_value;
read(dict, read_value);

assert(read_value == origin_value);
```

## Writing and reading to/from JSON (`dict_json.hpp` file)

Your dictionary should be able to be written to and read from a JSON file.
There is no need to create your own library for manipulating with JSON files.
Instead, you can use any **header-only** existing library, for example the [RapidJSON](https://rapidjson.org/).
In this case, the library files must be placed in the repository along with your code, and included with a relative path. When reading from JSON, we will assume that there are only 5 types:

1. instance (`dict_t`),
2. integer (`int`),
3. real number (`double`),
4. boolean value (bool: `true`/`false`) and
5. string (`std::string`).

When writing to JSON, it will almost be the same, but any integer (`signed`/`unsigned char`/`short`/`long`, except `bool`) is converted to `int`, other numbers are converted to `double`.

Read and write functions:

```cpp
void save_to_json  (std::ostream& os, dict);
bool load_from_json(std::istream& is, dict);
```
