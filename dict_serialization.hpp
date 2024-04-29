#pragma once

// `dict_serialization.hpp` part of header only solution.


#include <sstream>
#include "dict.hpp"


using namespace utils;

std::string index_to_string(size_t index) {
    std::ostringstream oss;
    oss << index;
    return oss.str();
}

template<typename T>
void write(dict_t &dict, const std::vector<T> &vec) {
    for (std::size_t i = 0; i < vec.size(); i++) {
        dict.put(index_to_string(i), vec[i]);
    }
}

template<typename T>
void write(dict_t &dict, const std::map<std::string, T> &map) {
    for (auto &[key, value]: map) {
        dict.put(key, value);
    }
}

template<typename T>
void write(dict_t &dict, const std::vector<std::vector<T>> &vec) {
    for (std::size_t i = 0; i < vec.size(); i++) {
        dict_t inner;
        write(inner, vec[i]);
        dict.put(index_to_string(i), std::move(inner));
    }
}

template<typename T>
void write(dict_t &dict, const std::vector<std::map<std::string, T>> &vec) {
    for (std::size_t i = 0; i < vec.size(); i++) {
        dict_t inner;
        write(inner, vec[i]);
        dict.put(index_to_string(i), std::move(inner));
    }
}

template<typename T>
void write(dict_t &dict, const std::map<std::string, std::vector<T>> &map) {
    for (auto &[key, value]: map) {
        dict_t inner;
        write(inner, value);
        dict.put(key, std::move(inner));
    }
}

template<typename T>
void write(dict_t &dict, const std::map<std::string, std::map<std::string, T>> &map) {
    for (auto &[key, value]: map) {
        dict_t inner;
        write(inner, value);
        dict.put(key, std::move(inner));
    }
}


template<typename T>
void read(const dict_t &dict, std::vector<T> &vec) {
    for (auto it: dict) {
        vec.push_back(dict.get<T>(it.first));
    }
}

template<typename T>
void read(const dict_t &dict, std::map<std::string, T> &map) {
    for (auto it: dict) {
        map[it.first] = dict.get<T>(it.first);
    }
}

template<typename T>
void read(const dict_t &dict, std::vector<std::vector<T>> &vec) {
    for (const auto &it: dict) {
        std::vector<T> inner;
        read(dict.get<dict_t>(it.first), inner);
        vec.push_back(inner);
    }
}

template<typename T>
void read(const dict_t &dict, std::vector<std::map<std::string, T>> &vec) {
    for (const auto &it: dict) {
        std::map<std::string, T> inner;
        read(dict.get<dict_t>(it.first), inner);
        vec.push_back(inner);
    }
}

template<typename T>
void read(const dict_t &dict, std::map<std::string, std::vector<T>> &map) {
    for (auto it: dict) {
        std::vector<T> inner;
        read(dict.get<dict_t>(it.first), inner);
        map[it.first] = inner;
    }
}

template<typename T>
void read(const dict_t &dict, std::map<std::string, std::map<std::string, T>> &map) {
    for (auto it: dict) {
        std::map<std::string, T> inner;
        read(dict.get<dict_t>(it.first), inner);
        map.emplace(it.first, inner);
    }
}
