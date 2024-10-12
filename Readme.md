# Fast C++23 JSON Parser

## Overview

This project implements a fast and efficient JSON parser in C++23. It provides a lightweight, header-only library for parsing JSON documents with a focus on performance and modern C++ features.

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
