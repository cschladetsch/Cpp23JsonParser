#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "json_parser.hpp"
#include "value.hpp"  // Ensure this is the file that defines the Value class

using namespace json;  // Use the json namespace

TEST_CASE("Parsing valid JSON objects", "[json]") {
    std::string json_string = R"({
        "name": "Alice",
        "age": 30,
        "is_student": false,
        "address": {
            "city": "Wonderland",
            "zipcode": "12345"
        },
        "grades": [85, 90, 92]
    })";

    Parser parser;  // Create a Parser instance
    Value parsed = parser.parse(json_string);  // Use the instance to call parse

    const auto& obj = parsed.get<Value::Object>();

    REQUIRE(obj.at("name").get<std::string>() == "Alice");
    REQUIRE(obj.at("age").get<double>() == 30);
    REQUIRE_FALSE(obj.at("is_student").get<bool>());
    REQUIRE(obj.at("address").get<Value::Object>().at("city").get<std::string>() == "Wonderland");
    REQUIRE(obj.at("grades").get<Value::Array>()[0].get<double>() == 85);
}

