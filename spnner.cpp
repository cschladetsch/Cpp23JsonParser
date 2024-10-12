// test_json_parser.cpp
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

    // Other test sections here...
}

