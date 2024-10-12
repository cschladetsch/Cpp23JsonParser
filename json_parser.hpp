#ifndef JSON_PARSER_HPP
#define JSON_PARSER_HPP

#include <string>
#include <map>
#include <vector>
#include <stdexcept>
#include <variant>

namespace json {

    class Value {
    public:
        enum class Type { Null, Boolean, Number, String, Object, Array };

        using Object = std::map<std::string, Value>;
        using Array = std::vector<Value>;

        Value() : type_(Type::Null) {}
        explicit Value(bool b) : type_(Type::Boolean), boolean_value(b) {}
        explicit Value(double n) : type_(Type::Number), number_value(n) {}
        explicit Value(const std::string& s) : type_(Type::String), string_value(s) {}
        explicit Value(Object obj) : type_(Type::Object), object_value(std::move(obj)) {}
        explicit Value(Array arr) : type_(Type::Array), array_value(std::move(arr)) {}

        Type type() const { return type_; }
        bool as_bool() const { if (type_ != Type::Boolean) throw std::runtime_error("Not a boolean"); return boolean_value; }
        double as_number() const { if (type_ != Type::Number) throw std::runtime_error("Not a number"); return number_value; }
        const std::string& as_string() const { if (type_ != Type::String) throw std::runtime_error("Not a string"); return string_value; }
        const Object& as_object() const { if (type_ != Type::Object) throw std::runtime_error("Not an object"); return object_value; }
        const Array& as_array() const { if (type_ != Type::Array) throw std::runtime_error("Not an array"); return array_value; }

        // Template method to get the stored value by type
        template<typename T>
        const T& get() const;

    private:
        Type type_;
        bool boolean_value = false;
        double number_value = 0.0;
        std::string string_value;
        Object object_value;
        Array array_value;
    };

    template<>
    inline const Value::Object& Value::get<Value::Object>() const {
        return as_object();
    }

    template<>
    inline const Value::Array& Value::get<Value::Array>() const {
        return as_array();
    }

    template<>
    inline const std::string& Value::get<std::string>() const {
        return as_string();
    }

    template<>
    inline const double& Value::get<double>() const {
        return as_number();
    }

    template<>
    inline const bool& Value::get<bool>() const {
        return as_bool();
    }

    class Parser {
    public:
        static Value parse(const std::string& json_string);

    private:
        static void skip_whitespace(const char*& json, size_t& pos);
        static Value parse_value(const char* json, size_t& pos);
        static Value parse_object(const char* json, size_t& pos);
        static Value parse_array(const char* json, size_t& pos);
        static Value parse_string(const char* json, size_t& pos);
        static Value parse_number(const char* json, size_t& pos);
        static Value parse_boolean(const char* json, size_t& pos);
        static Value parse_null(const char* json, size_t& pos);
    };

} // namespace json

#endif // JSON_PARSER_HPP

