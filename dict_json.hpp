#pragma once


// `dict_json.hpp` part of header only solution.
#include "include/rapidjson/document.h"
#include "include/rapidjson/writer.h"
#include "include/rapidjson/reader.h"
#include "include/rapidjson/stringbuffer.h"
#include "include/rapidjson/istreamwrapper.h"
#include "dict.hpp"

struct bad_format_json : public std::exception {
    [[nodiscard]] const char *what() const noexcept override {
        return "The JSON is ill formed\n";
    }
};

using namespace rapidjson;
using namespace utils;

void write_to_json(const dict_t &dict, Value &doc, Document::AllocatorType &allocator) {
    for (auto [key, value]: dict) {

        Value Key(key.c_str(), allocator);

        if (auto ptr = dict.get_ptr<dict_t>(key)) {
            Value inner(kObjectType);
            write_to_json(*ptr, inner, allocator);
            doc.AddMember(Key, inner, allocator);
            continue;
        }

        if (auto ptr = dict.get_ptr<std::string>(key)) {
            Value string_value(ptr->c_str(), allocator);
            doc.AddMember(Key, string_value, allocator);
            continue;
        }

        if (auto ptr = dict.get_ptr<bool>(key)) {
            doc.AddMember(Key, *ptr, allocator);
            continue;
        }

        try {
            int x = dict.get_int_key(key).value();
            doc.AddMember(Key, x, allocator);
            continue;
        } catch (std::bad_optional_access &) {}

        try {
            double x = dict.get_double_key(key).value();
            doc.AddMember(Key, x, allocator);
            continue;
        } catch (std::bad_optional_access &) {}

        throw bad_format_json();
    }
}

void save_to_json(std::ostream &os, const dict_t &dict) {
    Document doc;
    doc.SetObject();
    Document::AllocatorType &allocator = doc.GetAllocator();
    write_to_json(dict, doc, allocator);
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    doc.Accept(writer);
    os << buffer.GetString();
}

void read_json(const rapidjson::Value &json_value, dict_t &dict) {
    for (auto it = json_value.MemberBegin(); it != json_value.MemberEnd(); ++it) {
        const std::string key = it->name.GetString();
        const rapidjson::Value &value = it->value;

        if (value.IsObject()) {
            dict_t inner;
            read_json(value, inner);
            dict.put(key, std::move(inner));
        } else if (value.IsInt()) {
            dict.put(key, value.GetInt());
        } else if (value.IsDouble()) {
            dict.put(key, value.GetDouble());
        } else if (value.IsBool()) {
            dict.put(key, value.GetBool());
        } else if (value.IsString()) {
            dict.put(key, std::string(value.GetString()));
        } else {
            throw bad_format_json();
        }
    }
}

bool load_from_json(std::istream &is, dict_t &dict) {
    rapidjson::IStreamWrapper isw(is);
    rapidjson::Document document;
    document.ParseStream(isw);

    if (document.HasParseError()) {
        std::cerr << "An error occurred while trying to parse the JSON" << document.GetParseError() << "\n";
        return false;
    }
    read_json(document, dict);
    return true;
}
