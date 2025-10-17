# ArgParse Library

A Python-inspired C++17 argument parsing library with comprehensive features and robust error handling.

## Features

- ✅ **All Argument Types**: BOOL, INT, FLOAT, STR with automatic type validation
- ✅ **Positional & Optional Arguments**: Support for both with flexible ordering
- ✅ **Multiple Aliases**: `-v`, `--verbose`, `--verb` all point to same argument
- ✅ **Default Values**: Set defaults that can be overridden
- ✅ **Required Arguments**: Enforce mandatory arguments with validation
- ✅ **Choices Validation**: Restrict values to predefined options
- ✅ **nargs Support**: Python-style multiple values (`*`, `+`, `?`, numbers)
- ✅ **Help System**: Automatic help generation with `-h`/`--help`
- ✅ **Negative Numbers**: Proper handling of negative values
- ✅ **Error Handling**: Comprehensive validation with clear error messages

## Quick Start

```cpp
#include "lib/argparse.h"
using namespace ArgParse;

int main() {
    ArgumentParser parser("myprogram");
    
    // Add arguments
    parser.add_argument({"input"}, "Input file", STR, "", true);
    parser.add_argument({"-v", "--verbose"}, "Verbose mode", BOOL);
    parser.add_argument({"--count"}, "Number of items", INT, "10");
    
    // Parse command line
    std::vector<std::string> args = {"myprogram", "data.txt", "--verbose", "--count", "5"};
    if (parser.parse_args(args) != 0) return 1;
    
    // Access values
    std::string input = parser.get<std::string>("input");
    bool verbose = parser.get<bool>("verbose");
    int count = parser.get<int>("count");
    
    return 0;
}
```

## Common Usage Examples

### Basic Arguments
```cpp
// Positional argument
parser.add_argument({"filename"}, "Input file", STR, "", true);

// Optional with default
parser.add_argument({"--threads"}, "Thread count", INT, "4");

// Boolean flag
parser.add_argument({"-v", "--verbose"}, "Enable verbose output", BOOL);
```

### Choices and Validation
```cpp
// Restrict to specific values
parser.add_argument({"--format"}, "Output format", STR, "", false, "", 
                   {"json", "xml", "csv"});

// Required choice
parser.add_argument({"command"}, "Command to run", STR, "", true, "", 
                   {"start", "stop", "restart"});
```

### nargs - Multiple Values
```cpp
// Zero or more files
parser.add_argument({"--files"}, "Input files", STR, "", false, "", {}, "", "*");
auto files = parser.get_list<std::string>("files");

// One or more numbers
parser.add_argument({"--nums"}, "Numbers to process", INT, "", false, "", {}, "", "+");
auto numbers = parser.get_list<int>("nums");

// Optional config file
parser.add_argument({"--config"}, "Config file", STR, "", false, "", {}, "", "?");

// Exactly 2 coordinates
parser.add_argument({"--coords"}, "X Y coordinates", FLOAT, "", false, "", {}, "", "2");
auto coords = parser.get_list<float>("coords");
```

### Complex Example
```cpp
ArgumentParser parser("image_processor");

// Positional arguments
parser.add_argument({"input"}, "Input image file", STR, "", true);
parser.add_argument({"output"}, "Output image file", STR, "", true);

// Optional arguments with choices
parser.add_argument({"--format"}, "Output format", STR, "png", false, "", 
                   {"png", "jpg", "bmp"});
parser.add_argument({"--quality"}, "JPEG quality", INT, "90");
parser.add_argument({"-v", "--verbose"}, "Verbose output", BOOL);

// Multiple values
parser.add_argument({"--resize"}, "Width Height", INT, "", false, "", {}, "", "2");
parser.add_argument({"--filters"}, "Apply filters", STR, "", false, "", 
                   {"blur", "sharpen", "sepia"}, "", "*");

// Parse arguments
if (parser.parse_args(argc, argv) != 0) return 1;

// Access values
std::string input = parser.get<std::string>("input");
std::string output = parser.get<std::string>("output");
std::string format = parser.get<std::string>("format");
int quality = parser.get<int>("quality");
bool verbose = parser.get<bool>("verbose");

// Multiple values
auto resize = parser.get_list<int>("resize");  // [width, height]
auto filters = parser.get_list<std::string>("filters");  // ["blur", "sepia", ...]
```

### Command Line Examples
```bash
# Basic usage
./myprogram input.txt --verbose --count 5

# With choices
./image_processor input.jpg output.png --format png --quality 95

# Multiple values
./processor --files a.txt b.txt c.txt --nums 1 2 3 4

# nargs examples
./app --coords 10.5 20.3              # exactly 2 values
./app --filters blur sharpen           # one or more
./app --optional-files file1.txt       # zero or one
./app --all-files *.txt                # zero or more

# Help
./myprogram --help
./myprogram -h
```

## Error Handling

The library provides comprehensive error detection:

```cpp
int result = parser.parse_args(args);
if (result != 0) {
    // Parsing failed - error already printed to stderr
    return 1;
}

// Safe value access with type checking
try {
    int value = parser.get<int>("number");
} catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
}
```

## API Reference

### ArgumentParser Methods
- `add_argument(aliases, help, type, default, required, key, choices, metavar, nargs)`
- `parse_args(argc, argv)` or `parse_args(vector<string>)`
- `get<Type>(key)` - Get single value
- `get_list<Type>(key)` - Get multiple values (for nargs)

### Argument Types
- `BOOL` - Boolean flags
- `INT` - Integer numbers
- `FLOAT` - Floating-point numbers  
- `STR` - String values

### nargs Options
- `""` or `"1"` - Exactly one value (default)
- `"?"` - Zero or one value
- `"*"` - Zero or more values
- `"+"` - One or more values
- `"N"` - Exactly N values (e.g., `"2"`, `"3"`)

## Build & Integration

```bash
# Build the library
make

# Link in your project
g++ -std=c++17 myapp.cpp -Ipath/to/argparse -Lpath/to/build -largparse
```

## License

This library is part of the VortexDebug project.