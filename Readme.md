# Fast C++23 JSON Parser

## Overview

This project implements a fast and efficient JSON parser in C++23. It provides a lightweight, header-only library for parsing JSON documents with a focus on performance and modern C++ features.

## Sample

```bash
❯ ./r
===> Removing existing build directory...
===> Creating build directory...
===> Running CMake...
-- The C compiler identification is GNU 13.1.0
-- The CXX compiler identification is GNU 13.1.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /usr/bin/gcc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/g++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done (5.4s)
-- Generating done (0.0s)
-- Build files have been written to: /home/xian/local/repos/Cpp23Json/build
===> Building the project...
[ 16%] Building CXX object CMakeFiles/json_parser_lib.dir/json_parser.cpp.o
[ 33%] Linking CXX static library libjson_parser_lib.a
[ 33%] Built target json_parser_lib
[ 50%] Building CXX object CMakeFiles/json_parser_driver.dir/main.cpp.o
[ 66%] Linking CXX executable json_parser_driver
[ 66%] Built target json_parser_driver
[ 83%] Building CXX object CMakeFiles/test_json_parser.dir/test_json_parser.cpp.o
[100%] Linking CXX executable test_json_parser
[100%] Built target test_json_parser
===> Running tests...
===============================================================================
All tests passed (3 assertions in 1 test case)
```

## Features

- Compliant with JSON specification
- Utilizes C++23 features for improved performance and readability
- Header-only library for easy integration
- Supports all JSON data types: null, boolean, number, string, array, and object
- Exception-based error handling for robust parsing
- Efficient memory usage with `std::string_view` for zero-copy parsing
- Fast number parsing using `std::from_chars`

## Requirements

- C++23 compatible compiler (GCC 11 or later recommended)
- CMake 3.20 or later

## Building the Project

1. Clone the repository:
   ```
   git clone https://github.com/cschladetsch/fast-cpp23-json-parser.git
   cd fast-cpp23-json-parser
   ```

2. Create a build directory:
   ```
   mkdir build && cd build
   ```

3. Run CMake:
   ```
   cmake ..
   ```

4. Build the project:
   ```
   make
   ```

5. (Optional) Run the tests:
   ```
   make test
   ```

### Or just use 'r'

```bash
$ ./r
```

Will clean and build and run all tests.

## Usage

Here's a simple example of how to use the JSON parser:

```cpp
#include "json_parser.hpp"
#include <iostream>

int main() {
    std::string json_str = R"(
        {
            "name": "John Doe",
            "age": 30,
            "is_student": false,
            "grades": [85, 90, 92]
        }
    )";

    try {
        auto parsed = json::Parser::parse(json_str);
        auto& obj = parsed.get<json::Object>();
        
        std::cout << "Name: " << obj["name"].get<std::string>() << std::endl;
        std::cout << "Age: " << obj["age"].get<double>() << std::endl;
        std::cout << "Is student: " << (obj["is_student"].get<bool>() ? "Yes" : "No") << std::endl;
        
        std::cout << "Grades: ";
        for (const auto& grade : obj["grades"].get<json::Array>()) {
            std::cout << grade.get<double>() << " ";
        }
        std::cout << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
```

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Inspired by various JSON parsing libraries and techniques
- Thanks to the C++ community for continuous improvements to the language
