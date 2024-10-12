#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "json_parser.hpp"

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

    json::Value parsed = json::Parser::parse(json_string);
    const auto& obj = parsed.get<json::Value::Object>();

    REQUIRE(obj.at("name").get<std::string>() == "Alice");
    REQUIRE(obj.at("age").get<double>() == 30);
    REQUIRE_FALSE(obj.at("is_student").get<bool>());
    REQUIRE(obj.at("address").get<json::Value::Object>().at("city").get<std::string>() == "Wonderland");
    REQUIRE(obj.at("grades").get<json::Value::Array>()[0].get<double>() == 85);
}

TEST_CASE("Handling invalid JSON", "[json]") {
    std::string invalid_json = R"({"name": "Alice", "age": })";
    REQUIRE_THROWS_AS(json::Parser::parse(invalid_json), std::runtime_error);
}

TEST_CASE("Handling null values in JSON", "[json]") {
    std::string json_string = R"({
        "name": "Alice",
        "middle_name": null,
        "is_student": false
    })";

    json::Value parsed = json::Parser::parse(json_string);
    const auto& obj = parsed.get<json::Value::Object>();

    REQUIRE(obj.at("middle_name").is_null());
}

TEST_CASE("Handling empty objects and arrays", "[json]") {
    std::string json_string = R"({
        "empty_array": [],
        "empty_object": {}
    })";

    json::Value parsed = json::Parser::parse(json_string);
    const auto& obj = parsed.get<json::Value::Object>();

    REQUIRE(obj.at("empty_array").get<json::Value::Array>().empty());
    REQUIRE(obj.at("empty_object").get<json::Value::Object>().empty());
}

