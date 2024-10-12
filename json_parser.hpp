#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <variant>
#include <string_view>

namespace json {

class Value;

using Object = std::unordered_map<std::string, Value>;
using Array = std::vector<Value>;

class Value {
public:
    using variant_type = std::variant<std::nullptr_t, bool, double, std::string, Array, Object>;
    
    Value() : m_value(nullptr) {}
    Value(std::nullptr_t) : m_value(nullptr) {}
    Value(bool b) : m_value(b) {}
    Value(double d) : m_value(d) {}
    Value(const std::string& s) : m_value(s) {}
    Value(const Array& a) : m_value(a) {}
    Value(const Object& o) : m_value(o) {}

    template<typename T>
    const T& get() const {
        return std::get<T>(m_value);
    }

    const variant_type& get_variant() const { return m_value; }

private:
    variant_type m_value;
};

class Parser {
public:
    static Value parse(std::string_view json);

private:
    Parser(std::string_view json);
    Value parse_value();
    Value parse_null();
    Value parse_bool();
    Value parse_string();
    Value parse_number();
    Value parse_array();
    Value parse_object();
    void skip_whitespace();
    char current() const;

    std::string_view m_json;
    size_t m_pos;
};

} // namespace json
