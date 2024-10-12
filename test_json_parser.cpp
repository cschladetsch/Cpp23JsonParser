#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "json_parser.hpp"
#include <cmath>

TEST_CASE("JSON Parser Tests", "[json]") {
    SECTION("Parse null") {
        auto result = json::Parser::parse("null");
        REQUIRE(std::holds_alternative<std::nullptr_t>(result.get_variant()));
    }

    SECTION("Parse booleans") {
        SECTION("true") {
            auto true_result = json::Parser::parse("true");
            REQUIRE(true_result.get<bool>() == true);
        }
        SECTION("false") {
            auto false_result = json::Parser::parse("false");
            REQUIRE(false_result.get<bool>() == false);
        }
    }

    SECTION("Parse numbers") {
        SECTION("Integer") {
            auto int_result = json::Parser::parse("42");
            REQUIRE(int_result.get<double>() == Approx(42.0));
        }
        SECTION("Float") {
            auto float_result = json::Parser::parse("3.14");
            REQUIRE(float_result.get<double>() == Approx(3.14));
        }
        SECTION("Negative") {
            auto negative_result = json::Parser::parse("-123.456");
            REQUIRE(negative_result.get<double>() == Approx(-123.456));
        }
        SECTION("Exponent") {
            auto exponent_result = json::Parser::parse("1.23e-4");
            REQUIRE(exponent_result.get<double>() == Approx(1.23e-4));
        }
        SECTION("Large number") {
            auto large_number = json::Parser::parse("1.23456789e+100");
            REQUIRE(large_number.get<double>() == Approx(1.23456789e+100));
        }
    }

    SECTION("Parse strings") {
        SECTION("Simple string") {
            auto simple_string = json::Parser::parse("\"Hello, World!\"");
            REQUIRE(simple_string.get<std::string>() == "Hello, World!");
        }
        SECTION("Escaped characters") {
            auto escaped_string = json::Parser::parse("\"Escaped \\\" characters \\n \\t\"");
            REQUIRE(escaped_string.get<std::string>() == "Escaped \" characters \n \t");
        }
        SECTION("Unicode characters") {
            auto unicode_string = json::Parser::parse("\"Unicode: \\u00A9 \\u20AC\"");
            REQUIRE(unicode_string.get<std::string>() == "Unicode: © €");
        }
        SECTION("Empty string") {
            auto empty_string = json::Parser::parse("\"\"");
            REQUIRE(empty_string.get<std::string>().empty());
        }
        SECTION("Special characters") {
            auto special_chars = json::Parser::parse("\"!@#$%^&*()_+{}[]|:;<>,.?/~`\"");
            REQUIRE(special_chars.get<std::string>() == "!@#$%^&*()_+{}[]|:;<>,.?/~`");
        }
    }

    SECTION("Parse arrays") {
        SECTION("Empty array") {
            auto empty_array = json::Parser::parse("[]");
            REQUIRE(empty_array.get<json::Array>().empty());
        }
        SECTION("Number array") {
            auto number_array = json::Parser::parse("[1, 2, 3]");
            const auto& num_arr = number_array.get<json::Array>();
            REQUIRE(num_arr.size() == 3);
            REQUIRE(num_arr[0].get<double>() == Approx(1.0));
            REQUIRE(num_arr[1].get<double>() == Approx(2.0));
            REQUIRE(num_arr[2].get<double>() == Approx(3.0));
        }
        SECTION("Mixed array") {
            auto mixed_array = json::Parser::parse("[1, \"two\", true, null]");
            const auto& mix_arr = mixed_array.get<json::Array>();
            REQUIRE(mix_arr.size() == 4);
            REQUIRE(mix_arr[0].get<double>() == Approx(1.0));
            REQUIRE(mix_arr[1].get<std::string>() == "two");
            REQUIRE(mix_arr[2].get<bool>() == true);
            REQUIRE(std::holds_alternative<std::nullptr_t>(mix_arr[3].get_variant()));
        }
        SECTION("Nested array") {
            auto nested_array = json::Parser::parse("[[1, 2], [3, 4]]");
            const auto& nest_arr = nested_array.get<json::Array>();
            REQUIRE(nest_arr.size() == 2);
            REQUIRE(nest_arr[0].get<json::Array>()[0].get<double>() == Approx(1.0));
            REQUIRE(nest_arr[1].get<json::Array>()[1].get<double>() == Approx(4.0));
        }
        SECTION("Large array") {
            auto large_array = json::Parser::parse(std::string("[") + std::string(1000, '0') + "]");
            REQUIRE(large_array.get<json::Array>().size() == 1000);
        }
    }

    SECTION("Parse objects") {
        SECTION("Empty object") {
            auto empty_object = json::Parser::parse("{}");
            REQUIRE(empty_object.get<json::Object>().empty());
        }
        SECTION("Simple object") {
            auto simple_object = json::Parser::parse("{\"name\": \"John\", \"age\": 30}");
            const auto& simple_obj = simple_object.get<json::Object>();
            REQUIRE(simple_obj.size() == 2);
            REQUIRE(simple_obj.at("name").get<std::string>() == "John");
            REQUIRE(simple_obj.at("age").get<double>() == Approx(30.0));
        }
        SECTION("Nested object") {
            auto nested_object = json::Parser::parse("{\"person\": {\"name\": \"Alice\", \"age\": 25}}");
            const auto& nested_obj = nested_object.get<json::Object>();
            REQUIRE(nested_obj.size() == 1);
            const auto& person = nested_obj.at("person").get<json::Object>();
            REQUIRE(person.at("name").get<std::string>() == "Alice");
            REQUIRE(person.at("age").get<double>() == Approx(25.0));
        }
        SECTION("Object with array") {
            auto object_with_array = json::Parser::parse("{\"numbers\": [1, 2, 3]}");
            const auto& obj_arr = object_with_array.get<json::Object>();
            REQUIRE(obj_arr.at("numbers").get<json::Array>().size() == 3);
        }
        SECTION("Complex object") {
            auto complex_object = json::Parser::parse(R"(
                {
                    "name": "Complex Object",
                    "values": [1, 2, 3],
                    "nested": {
                        "a": true,
                        "b": false
                    },
                    "mixed": [1, {"x": 10}, [true, null]]
                }
            )");
            const auto& complex_obj = complex_object.get<json::Object>();
            REQUIRE(complex_obj.size() == 4);
            REQUIRE(complex_obj.at("name").get<std::string>() == "Complex Object");
            REQUIRE(complex_obj.at("values").get<json::Array>().size() == 3);
            REQUIRE(complex_obj.at("nested").get<json::Object>().size() == 2);
            REQUIRE(complex_obj.at("mixed").get<json::Array>().size() == 3);
        }
    }

    SECTION("Error handling") {
        SECTION("Invalid JSON") {
            REQUIRE_THROWS_AS(json::Parser::parse(""), std::runtime_error);
            REQUIRE_THROWS_AS(json::Parser::parse("invalid"), std::runtime_error);
            REQUIRE_THROWS_AS(json::Parser::parse("{\"key\": }"), std::runtime_error);
            REQUIRE_THROWS_AS(json::Parser::parse("[1, 2, ]"), std::runtime_error);
            REQUIRE_THROWS_AS(json::Parser::parse("\"unclosed string"), std::runtime_error);
        }
    }
}
