#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <variant>
#include <optional>

namespace custom_json {

class Value {
public:
    using Array = std::vector<Value>;
    using Object = std::unordered_map<std::string, Value>;

    enum class Type {
        Null,
        Boolean,
        Number,
        String,
        Array,
        Object
    };

private:
    std::variant<std::monostate, bool, double, std::string, Array, Object> data;

public:
    Value() : data(std::monostate{}) {}
    Value(bool b) : data(b) {}
    Value(double d) : data(d) {}
    Value(const std::string& s) : data(s) {}
    Value(const Array& a) : data(a) {}
    Value(const Object& o) : data(o) {}

    Type type() const {
        return static_cast<Type>(data.index());
    }

    template<typename T>
    const T& get() const {
        return std::get<T>(data);
    }

    const std::string& as_string() const {
        return get<std::string>();
    }

    double as_number() const {
        return get<double>();
    }

    bool as_bool() const {
        return get<bool>();
    }

    const Array& as_array() const {
        return get<Array>();
    }

    const Object& as_object() const {
        return get<Object>();
    }
};

Value parse(const std::string& json_string);

} // namespace custom_json
