#pragma once

// `dict.hpp` part of header only solution.

#include <string>
#include <map>
#include <vector>
#include <memory>
#include <stdexcept>
#include <iostream>
#include <initializer_list>
#include <optional>

template<typename T>
struct is_in_place_type : std::false_type {
};

template<typename T>
struct is_in_place_type<std::in_place_type_t<T>> : std::true_type {
};

class TypeErased {

public:
    constexpr TypeErased() = default;

    template<typename T, typename = std::enable_if_t<
            !std::is_same_v<std::decay_t<T>, TypeErased> &&
            !is_in_place_type<std::decay_t<T>>::value &&
            std::is_copy_constructible_v<std::decay_t<T>>
    >>
    TypeErased(T &&value) {
        static_assert(std::is_copy_constructible_v<std::decay_t<T>>, "program is ill-formed");
        *this = std::forward<T>(value);
    }

    TypeErased(const TypeErased &other) {
        if (other.instance) {
            instance = other.instance->clone();
        }
    }

    TypeErased(TypeErased &&other) noexcept
            : instance(std::move(other.instance)) {
    }

    TypeErased(std::initializer_list<std::pair<const std::string, TypeErased>> list);

    template<typename T, typename =  std::enable_if_t<
            !std::is_same_v<std::decay_t<T>, TypeErased> &&
            std::is_copy_constructible_v<std::decay_t<T>>>>
    TypeErased &operator=(T &&value) {
        instance = std::make_unique<Model < std::decay_t<T>> > (std::forward<T>(value));
        return *this;
    }

    TypeErased &operator=(const TypeErased &rhs) {
        TypeErased(rhs).swap(*this);
        return *this;
    }

    TypeErased &operator=(TypeErased &&rhs) noexcept {
        TypeErased(std::move(rhs)).swap(*this);
        return *this;
    }

    bool operator==(const TypeErased &other) const {
        return (instance && other.instance) &&
               type() == other.type() &&
               (*instance == *other.instance);
    }

    template<typename T>
    const T *getObjectPtr() const {
        if (type() == typeid(T)) {
            const Model <T> *aux = dynamic_cast<const Model <T> *>(instance.get());
            return (aux == nullptr) ? nullptr : &(aux->_data);
        }
        throw std::bad_cast();
    }

    template<typename T>
    T *getObjectPtr() {
        if (type() == typeid(T)) {
            Model <T> *aux = dynamic_cast<Model <T> *>(instance.get());
            return (aux == nullptr) ? nullptr : &(aux->_data);
        }
        throw std::bad_cast();
    }

private:

    struct Base {
        virtual ~Base() = default;

        [[nodiscard]] virtual const std::type_info &type() const = 0;

        virtual bool operator==(const Base &other) const = 0;

        [[nodiscard]] virtual std::unique_ptr<Base> clone() const = 0;
    };

    template<typename T>
    struct Model : Base {
        template<typename U, typename = std::enable_if_t<std::is_move_constructible_v<std::decay_t<U>>>>
        explicit Model(U &&t) : _data(std::forward<U>(t)) {
        }

        template<typename U>
        explicit Model(const U &t) : _data(t) {
        }

        [[nodiscard]] const std::type_info &type() const override {
            return typeid(T);
        }

        bool operator==(const Base &other) const override {
            if (type() == other.type()) {
                const auto *otherModel = dynamic_cast<const Model<T> *>(&other);
                if (otherModel) {
                    return _data == otherModel->_data;
                }
            }
            return false;
        }

        [[nodiscard]] std::unique_ptr<Base> clone() const override {
            return std::make_unique<Model<T>>(_data);
        }

        T _data;
    };

    void swap(TypeErased &other) noexcept {
        std::swap(instance, other.instance);
    }

    [[nodiscard]] const std::type_info &type() const {
        return instance->type();
    }

    std::unique_ptr<Base> instance;
};

namespace utils {

    struct no_key_exception : public std::exception {
        [[nodiscard]] const char *what() const noexcept override {
            return "Key not found";
        }
    };

    struct invalid_type_exception : public std::exception {
        [[nodiscard]] const char *what() const noexcept override {
            return "Invalid type for the key";
        }
    };

    class dict_t {
    public:

        dict_t() = default;

        dict_t(std::initializer_list<std::pair<const std::string, TypeErased>> list) {
            for (auto &[key, value]: list) {
                data[key] = value;
            }
        }

        dict_t(dict_t &&other) noexcept {
            *this = std::forward<dict_t>(other);
        }

        dict_t(const dict_t &other) {
            *this = other;
        }

        dict_t &operator=(const dict_t &other) {
            if (this != &other) {
                data.clear();
                try {
                    data = other.data;
                }
                catch (std::runtime_error &) {
                    data.clear();
                    throw std::runtime_error("Error copying the dictionary");
                }

            }
            return *this;
        }

        dict_t &operator=(dict_t &&other) noexcept {
            if (this != &other) {
                data = std::move(other.data);
                other.clear();
            }
            return *this;
        }

        bool put(const std::string &key, TypeErased &&value) {
            data[key] = std::move(value);
            return true;
        }

        bool put(const std::string &key, const TypeErased &value) {
            data[key] = value;
            return true;
        }

        template<typename T>
        [[nodiscard]] const T *get_ptr(const std::string &key) const {
            auto result = data.find(key);
            if (result == data.end()) {
                return nullptr;
            }
            try {
                return result->second.getObjectPtr<T>();
            }
            catch (std::bad_cast &) {
                return nullptr;
            }
        }

        template<typename T>
        const T &get(const std::string &key) const {
            const T *ptr = get_ptr<T>(key);
            if (!ptr) {
                if (data.find(key) == data.end()) {
                    throw no_key_exception();
                } else {
                    throw invalid_type_exception();
                }
            }
            return *ptr;
        }

        template<typename T>
        [[nodiscard]] T *get_ptr(const std::string &key) {
            auto result = data.find(key);
            if (result == data.end()) {
                return nullptr;
            }
            try {
                return result->second.getObjectPtr<T>();
            }
            catch (std::bad_cast &) {
                return nullptr;
            }
        }

        template<typename T>
        T &get(const std::string &key) {
            T *ptr = get_ptr<T>(key);
            if (!ptr) {
                if (data.find(key) == data.end()) {
                    throw no_key_exception();
                } else {
                    throw invalid_type_exception();
                }
            }
            return *ptr;
        }

        [[nodiscard]] std::optional<int> get_int_key(const std::string &key) const {
            return get_any_ptr<int, signed char, unsigned char, short int,
                    unsigned short int, unsigned int, long int, unsigned long int,
                    long long int, long long int, unsigned long long int>(key);
        }

        [[nodiscard]] std::optional<double> get_double_key(const std::string &key) const {
            return get_any_ptr<double, float, long double>(key);
        }

        bool remove(const std::string &key) {
            if (contains(key)) {
                data.erase(data.find(key));
                return true;
            }
            return false;
        }

        [[nodiscard]] bool contains(const std::string &key) const {
            return data.find(key) != data.end();
        }

        [[nodiscard]] bool empty() const {
            return data.empty();
        }

        [[nodiscard]] bool is_dict(const std::string &key) const {
            return get_ptr<dict_t>(key) != nullptr;
        }

        void clear() {
            data.clear();
        }

        bool operator==(const dict_t &other) const {
            return data == other.data;
        }

        bool operator!=(const dict_t &other) const {
            return !(*this == other);
        }

        std::map<std::string, TypeErased>::iterator begin() {
            return data.begin();
        }

        std::map<std::string, TypeErased>::iterator end() {
            return data.end();
        }

        [[nodiscard]] std::map<std::string, TypeErased>::const_iterator begin() const {
            return data.cbegin();
        }

        [[nodiscard]] std::map<std::string, TypeErased>::const_iterator end() const {
            return data.cend();
        }

    private:
        std::map<std::string, TypeErased> data;

        template<typename T, typename U, typename... Tail>
        [[nodiscard]] std::optional<T> get_any_ptr(const std::string &key) const {
            const U *ptr = get_ptr<U>(key);
            if (!ptr) {
                return get_any_ptr<T, Tail...>(key);
            }
            return static_cast<T>(*ptr);
        }

        template<typename T>
        [[nodiscard]] std::optional<T> get_any_ptr(const std::string &key) const {
            const T *ptr = get_ptr<T>(key);
            if (!ptr) {
                return std::nullopt;
            }
            return *ptr;
        }
    };

} // namespace utils

TypeErased::TypeErased(std::initializer_list<std::pair<const std::string, TypeErased>> list)
        : TypeErased(utils::dict_t(list)) {
}