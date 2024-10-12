#include "json_parser.hpp"
#include <cassert>
#include <iostream>

void test_json_parser() {
    // Parse null
    {
        auto result = json::Parser::parse("null");
        assert(std::holds_alternative<std::nullptr_t>(result.get_variant()));
    }

    // Parse boolean
    {
        auto true_result = json::Parser::parse("true");
        assert(true_result.get<bool>() == true);

        auto false_result = json::Parser::parse("false");
        assert(false_result.get<bool>() == false);
    }

    // Parse number
    {
        auto int_result = json::Parser::parse("42");
        assert(int_result.get<double>() == 42.0);

        auto float_result = json::Parser::parse("3.14");
        assert(float_result.get<double>() == 3.14);
    }

    // Parse string
    {
        auto result = json::Parser::parse("\"Hello, World!\"");
        assert(result.get<std::string>() == "Hello, World!");
    }

    // Parse array
    {
        auto result = json::Parser::parse("[1, 2, 3]");
        const auto& array = result.get<json::Array>();
        assert(array.size() == 3);
        assert(array[0].get<double>() == 1.0);
        assert(array[1].get<double>() == 2.0);
        assert(array[2].get<double>() == 3.0);
    }

    // Parse object
    {
        auto result = json::Parser::parse("{\"name\": \"John\", \"age\": 30}");
        const auto& object = result.get<json::Object>();
        assert(object.size() == 2);
        assert(object.at("name").get<std::string>() == "John");
        assert(object.at("age").get<double>() == 30.0);
    }

    std::cout << "All tests passed!" << std::endl;
}

int main() {
    test_json_parser();
    return 0;
}
