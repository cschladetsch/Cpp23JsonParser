#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "json_parser.hpp"

TEST_CASE("JSON Parser Tests", "[json]") {
    // Test parsing a simple JSON object
    std::string json_string = R"({
        "name": "Alice",
        "age": 30,
        "is_student": false
    })";

    json::Value parsed = json::Parser::parse(json_string);
    const auto& obj = parsed.get<json::Value::Object>();

    REQUIRE(obj.at("name").get<std::string>() == "Alice");
    REQUIRE(obj.at("age").get<double>() == 30);
    REQUIRE_FALSE(obj.at("is_student").get<bool>());
}

