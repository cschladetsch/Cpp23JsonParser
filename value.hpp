#pragma once
#include <string>
#include <variant>
#include <unordered_map>
#include <vector>

namespace json {

    class Value {
    public:
        using Object = std::unordered_map<std::string, Value>;
        using Array = std::vector<Value>;
        using Null = std::nullptr_t;

        Value() : value_(nullptr) {}
        Value(bool b) : value_(b) {}
        Value(int i) : value_(i) {}
        Value(double d) : value_(d) {}
        Value(const std::string& s) : value_(s) {}
        Value(std::string&& s) : value_(std::move(s)) {}
        Value(const Object& obj) : value_(obj) {}
        Value(Object&& obj) : value_(std::move(obj)) {}
        Value(const Array& arr) : value_(arr) {}
        Value(Array&& arr) : value_(std::move(arr)) {}
        Value(Null n) : value_(n) {}

        template<typename T>
        T& get() {
            return std::get<T>(value_);
        }

        template<typename T>
        const T& get() const {
            return std::get<T>(value_);
        }

    private:
        std::variant<Null, bool, int, double, std::string, Object, Array> value_;
    };
}

