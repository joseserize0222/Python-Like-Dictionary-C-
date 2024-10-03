
# Dynamic Dictionary Implementation

As part of my exploration into C++ data structures, I developed a dynamic dictionary called `utils::dict_t`. This dictionary uses `std::string` as the key type and supports storing values of any copyable or movable type, including another dictionary. The keys are unique, and this feature makes the dictionary highly adaptable for storing hierarchical structures like configurations. Additionally, it is particularly useful as an intermediate data structure for serializing C++ objects into various formats (JSON, XML, etc.), eliminating the need to write custom serializers for each format.

## Core Functionality

The dictionary provides several key operations that allow users to manage and retrieve data effectively.

### 1. Adding Values to the Dictionary
The `put` function adds a key-value pair to the dictionary. It preserves the value's category (copyable or movable), meaning that if a temporary instance is passed, it will not be copied but moved (if supported).

```cpp
bool put(dict, key, value);
```

### 2. Retrieving Values
To retrieve a value, I implemented two functions: `get_ptr` and `get`. These functions return a pointer or reference to the value associated with the key. If the key doesn't exist, or if the requested type does not match the stored type, `get_ptr` will return `nullptr` and `get` will throw either `utils::no_key_exception` or `utils::invalid_type_exception`.

```cpp
<pointer on T>   get_ptr(dict, key);
<reference on T> get    (dict, key);
```

### 3. Removing and Utility Functions
Several utility functions are available to remove a key, check if a key exists, clear the dictionary, and verify whether the dictionary is empty:

```cpp
bool remove  (dict, key);
bool contains(dict, key);
bool empty         (dict);
void clear         (dict);
```

### 4. Checking if a Key Represents Another Dictionary
I also implemented a function that checks if the value associated with a key is another dictionary:

```cpp
bool is_dict(dict, key);
```

### 5. Dictionary Comparisons
The dictionary supports equality and inequality comparison operators for checking if two dictionaries are identical.

---

## Advanced Features

### Initialization with Initializer Lists
The dictionary can be initialized using initializer lists, making it easy to create complex nested structures. For example:

```cpp
utils::dict_t dict = {
    {"five", 5},
    {"almost_zero", 0.01},
    {"inner_dict", {
        {"str_key", std::string("dict")},
        {"pi", 3.14}
    }}
};
```

### Exception Safety and Copyability
All operations guarantee strong exception safety, and the dictionary can be both copied and moved efficiently.

---

## Serialization

To extend its usefulness, I implemented serialization functionality to store objects in a dictionary. This is especially helpful when serializing C++ classes into hierarchical formats like JSON or XML.

The dictionary supports common C++ types, including arithmetic types, strings, and containers like `vector<?>` and `map<string, ?>`. If a map contains another map, it will be serialized as a dictionary within a dictionary.

### Writing to the Dictionary
To write an object into the dictionary, the following function is used:

```cpp
void write(dict, value);
```

### Reading from the Dictionary
Similarly, I implemented a function to read data from the dictionary back into its original form:

```cpp
void read(dict, value);
```

For example, consider the following composition of types:

```cpp
using composition_t = map<string, vector<map<string, int>>>;
composition_t origin_value = {
    {"one", {
        {},
        {{"single", 3}},
        {{"double_1", 1}, {"double_2", 2}}
    }},
    {"empty", {}}
};

dict_t dict;
write(dict, origin_value);

composition_t read_value;
read(dict, read_value);

assert(read_value == origin_value);
```

In this case, the `write` function serializes the nested structure into the dictionary, and `read` deserializes it back to its original form, ensuring that the original and read values are identical.

---

## JSON Serialization

One of the key features of the dictionary is the ability to serialize its contents into JSON format and read from JSON. For this, I used a header-only library like [RapidJSON](https://rapidjson.org/), ensuring that the dictionary can be serialized without needing to write custom JSON manipulation code.

When serializing to JSON, I convert data types in a straightforward manner:
- Integers are written as `int`,
- Real numbers as `double`,
- Booleans as `bool`,
- Strings as `std::string`.

For deserialization, the dictionary can recognize five types: `dict_t`, `int`, `double`, `bool`, and `std::string`.

### JSON Serialization Functions

To write a dictionary to a JSON file, the following function is used:

```cpp
void save_to_json(std::ostream& os, dict);
```

To read from a JSON file, the following function is available:

```cpp
bool load_from_json(std::istream& is, dict);
```

---

This project provided me with invaluable insights into advanced C++ features such as dynamic typing, serialization, and exception safety, while also improving my understanding of data structures. The dynamic dictionary structure I implemented is highly flexible and can be used in various applications, especially in configurations and data serialization scenarios.
