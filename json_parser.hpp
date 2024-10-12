#pragma once
#include <string>
#include <variant>
#include <vector>
#include <map>

namespace json {

    // JSON value class representing different possible types
    class Value {
    public:
        using Array = std::vector<Value>;
        using Object = std::map<std::string, Value>;

        // Constructors for different JSON value types
        Value() = default;
        Value(double num) : value_(num) {}
        Value(const std::string& str) : value_(str) {}
        Value(bool b) : value_(b) {}
        Value(const Array& arr) : value_(arr) {}
        Value(const Object& obj) : value_(obj) {}
        Value(std::nullptr_t) : value_(nullptr) {}

        // Template method to get the stored value with type T
        template<typename T>
        const T& get() const {
            return std::get<T>(value_);
        }

    private:
        std::variant<std::nullptr_t, double, std::string, bool, Array, Object> value_;
    };

    // JSON parser class
    class Parser {
    public:
        static Value parse(const std::string& json);
    };
}

