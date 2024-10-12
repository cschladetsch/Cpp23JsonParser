#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "json_parser.hpp"
#include "spinner.hpp"  // Include the spinner header
#include <cmath>

TEST_CASE("JSON Parser Tests", "[json]") {
    SECTION("Parse null") {
        auto result = json::Parser::parse("null");
        REQUIRE(std::holds_alternative<std::nullptr_t>(result.get_variant()));
    }

    SECTION("Parse boolean true") {
        auto result = json::Parser::parse("true");
        REQUIRE(std::holds_alternative<bool>(result.get_variant()));
        REQUIRE(result.get<bool>() == true);
    }

    SECTION("Parse boolean false") {
        auto result = json::Parser::parse("false");
        REQUIRE(std::holds_alternative<bool>(result.get_variant()));
        REQUIRE(result.get<bool>() == false);
    }

    SECTION("Parse number") {
        auto result = json::Parser::parse("123.45");
        REQUIRE(std::holds_alternative<double>(result.get_variant()));
        REQUIRE(result.get<double>() == Approx(123.45));
    }

    SECTION("Parse string") {
        auto result = json::Parser::parse(R"("hello world")");
        REQUIRE(std::holds_alternative<std::string>(result.get_variant()));
        REQUIRE(result.get<std::string>() == "hello world");
    }

    SECTION("Parse array") {
        auto result = json::Parser::parse(R"([1, 2, 3])");
        REQUIRE(std::holds_alternative<std::vector<json::Value>>(result.get_variant()));
        const auto& array = result.get<std::vector<json::Value>>();
        REQUIRE(array.size() == 3);
        REQUIRE(array[0].get<double>() == 1);
        REQUIRE(array[1].get<double>() == 2);
        REQUIRE(array[2].get<double>() == 3);
    }

    SECTION("Parse object") {
        auto result = json::Parser::parse(R"({"key": "value"})");
        REQUIRE(std::holds_alternative<std::map<std::string, json::Value>>(result.get_variant()));
        const auto& obj = result.get<std::map<std::string, json::Value>>();
        REQUIRE(obj.at("key").get<std::string>() == "value");
    }
}
