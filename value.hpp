#pragma once

#include <string>
#include <variant>
#include <vector>
#include <unordered_map>
#include <stdexcept>

namespace json {

class Value {
public:
    // Define the types that can be held by Value
    using Null = std::nullptr_t;
    using Boolean = bool;
    using Number = double;
    using String = std::string;
    using Array = std::vector<Value>;
    using Object = std::unordered_map<std::string, Value>;

    // Construct a Value from different types
    Value() : value_(nullptr) {}
    Value(Null) : value_(nullptr) {}
    Value(Boolean boolean) : value_(boolean) {}
    Value(Number number) : value_(number) {}
    Value(const String& string) : value_(string) {}
    Value(const Array& array) : value_(array) {}
    Value(const Object& object) : value_(object) {}

    // Get the stored value by type
    template <typename T>
    T& get() {
        if (!std::holds_alternative<T>(value_)) {
            throw std::runtime_error("Bad access");
        }
        return std::get<T>(value_);
    }

    template <typename T>
    const T& get() const {
        if (!std::holds_alternative<T>(value_)) {
            throw std::runtime_error("Bad access");
        }
        return std::get<T>(value_);
    }

    // Check if the value holds a specific type
    bool is_null() const { return std::holds_alternative<Null>(value_); }
    bool is_boolean() const { return std::holds_alternative<Boolean>(value_); }
    bool is_number() const { return std::holds_alternative<Number>(value_); }
    bool is_string() const { return std::holds_alternative<String>(value_); }
    bool is_array() const { return std::holds_alternative<Array>(value_); }
    bool is_object() const { return std::holds_alternative<Object>(value_); }

private:
    std::variant<Null, Boolean, Number, String, Array, Object> value_;
};

} // namespace json

