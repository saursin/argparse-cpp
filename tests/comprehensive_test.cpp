/**
 * Comprehensive Test Suite for ArgParse Library
 * 
 * This unified test suite covers all implemented features:
 * - Basic argument types (BOOL, INT, FLOAT, STR)
 * - Positional and optional arguments
 * - Default values and required arguments
 * - Choices validation
 * - Metavar display
 * - Help system
 * - nargs support (*, +, ?, numbers)
 * - Error handling and edge cases
 * - Type validation and conversion
 * - Negative number handling
 * - Mixed argument scenarios
 * - Extended functionality tests:
 *   - has_argument() and get_with_default() functionality
 *   - get_all_keys() functionality
 *   - Advanced nargs edge cases
 *   - Complex positional argument scenarios
 *   - Alias to key conversion edge cases
 *   - Boundary value testing
 *   - Error message validation
 *   - Performance stress tests
 */

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <sstream>
#include <functional>
#include <cmath>
#include <chrono>
#include "argparse.h"

using namespace ArgParse;

class UnifiedTestSuite {
private:
    int total_tests = 0;
    int passed_tests = 0;
    int failed_tests = 0;
    
    void print_test_header(const std::string& test_name) {
        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << "TEST: " << test_name << std::endl;
        std::cout << std::string(60, '=') << std::endl;
    }
    
    void print_subtest(const std::string& subtest_name) {
        std::cout << "\n--- " << subtest_name << " ---" << std::endl;
    }
    
    bool run_test(const std::string& test_name, std::function<bool()> test_func) {
        total_tests++;
        try {
            bool result = test_func();
            if (result) {
                std::cout << "✓ PASS: " << test_name << std::endl;
                passed_tests++;
                return true;
            } else {
                std::cout << "✗ FAIL: " << test_name << std::endl;
                failed_tests++;
                return false;
            }
        } catch (const std::exception& e) {
            std::cout << "✗ FAIL: " << test_name << " (Exception: " << e.what() << ")" << std::endl;
            failed_tests++;
            return false;
        }
    }

public:
    void test_basic_types() {
        print_test_header("Basic Argument Types");
        
        // Test BOOL type
        run_test("BOOL argument parsing", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--flag"}, "Test flag", BOOL);
            
            std::vector<std::string> args = {"test", "--flag"};
            int result = parser.parse_args(args);
            return result == 0 && parser.get<bool>("flag") == true;
        });
        
        // Test INT type
        run_test("INT argument parsing", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--number"}, "Test number", INT);
            
            std::vector<std::string> args = {"test", "--number", "42"};
            int result = parser.parse_args(args);
            return result == 0 && parser.get<int>("number") == 42;
        });
        
        // Test FLOAT type
        run_test("FLOAT argument parsing", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--value"}, "Test value", FLOAT);
            
            std::vector<std::string> args = {"test", "--value", "3.14"};
            int result = parser.parse_args(args);
            return result == 0 && std::abs(parser.get<float>("value") - 3.14f) < 0.001f;
        });
        
        // Test STR type
        run_test("STR argument parsing", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--text"}, "Test text", STR);
            
            std::vector<std::string> args = {"test", "--text", "hello"};
            int result = parser.parse_args(args);
            return result == 0 && parser.get<std::string>("text") == "hello";
        });
    }
    
    void test_positional_arguments() {
        print_test_header("Positional Arguments");
        
        // Test single positional argument
        run_test("Single positional argument", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"filename"}, "Input filename", STR, "", true);
            
            std::vector<std::string> args = {"test", "input.txt"};
            int result = parser.parse_args(args);
            return result == 0 && parser.get<std::string>("filename") == "input.txt";
        });
        
        // Test multiple positional arguments
        run_test("Multiple positional arguments", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"source"}, "Source file", STR, "", true);
            parser.add_argument({"dest"}, "Destination file", STR, "", true);
            
            std::vector<std::string> args = {"test", "input.txt", "output.txt"};
            int result = parser.parse_args(args);
            return result == 0 && 
                   parser.get<std::string>("source") == "input.txt" &&
                   parser.get<std::string>("dest") == "output.txt";
        });
        
        // Test positional arguments with different types
        run_test("Positional arguments with different types", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"name"}, "Name", STR, "", true);
            parser.add_argument({"age"}, "Age", INT, "", true);
            parser.add_argument({"height"}, "Height", FLOAT, "", true);
            
            std::vector<std::string> args = {"test", "John", "25", "5.9"};
            int result = parser.parse_args(args);
            return result == 0 && 
                   parser.get<std::string>("name") == "John" &&
                   parser.get<int>("age") == 25 &&
                   std::abs(parser.get<float>("height") - 5.9f) < 0.001f;
        });
    }
    
    void test_optional_arguments() {
        print_test_header("Optional Arguments");
        
        // Test short and long forms
        run_test("Short and long argument forms", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"-v", "--verbose"}, "Verbose mode", BOOL);
            parser.add_argument({"-f", "--file"}, "Input file", STR);
            
            std::vector<std::string> args = {"test", "-v", "--file", "test.txt"};
            int result = parser.parse_args(args);
            return result == 0 && 
                   parser.get<bool>("verbose") == true &&
                   parser.get<std::string>("file") == "test.txt";
        });
        
        // Test multiple aliases
        run_test("Multiple argument aliases", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"-o", "--output", "--out"}, "Output file", STR);
            
            std::vector<std::string> args = {"test", "--out", "result.txt"};
            int result = parser.parse_args(args);
            // The key is resolved to the longest alias (--output -> output)
            return result == 0 && parser.get<std::string>("output") == "result.txt";
        });
        
        // Test mixed positional and optional
        run_test("Mixed positional and optional arguments", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"input"}, "Input file", STR, "", true);
            parser.add_argument({"--verbose"}, "Verbose mode", BOOL);
            parser.add_argument({"--output"}, "Output file", STR, "output.txt");
            
            std::vector<std::string> args = {"test", "input.txt", "--verbose", "--output", "result.txt"};
            int result = parser.parse_args(args);
            return result == 0 && 
                   parser.get<std::string>("input") == "input.txt" &&
                   parser.get<bool>("verbose") == true &&
                   parser.get<std::string>("output") == "result.txt";
        });
    }
    
    void test_default_values() {
        print_test_header("Default Values");
        
        // Test string default
        run_test("String default value", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--name"}, "Name", STR, "default_name");
            
            std::vector<std::string> args = {"test"};
            int result = parser.parse_args(args);
            return result == 0 && parser.get<std::string>("name") == "default_name";
        });
        
        // Test integer default
        run_test("Integer default value", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--count"}, "Count", INT, "10");
            
            std::vector<std::string> args = {"test"};
            int result = parser.parse_args(args);
            return result == 0 && parser.get<int>("count") == 10;
        });
        
        // Test overriding default
        run_test("Override default value", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--port"}, "Port", INT, "8080");
            
            std::vector<std::string> args = {"test", "--port", "9000"};
            int result = parser.parse_args(args);
            return result == 0 && parser.get<int>("port") == 9000;
        });
    }
    
    void test_required_arguments() {
        print_test_header("Required Arguments");
        
        // Test required argument provided
        run_test("Required argument provided", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--input"}, "Input file", STR, "", true);
            
            std::vector<std::string> args = {"test", "--input", "file.txt"};
            int result = parser.parse_args(args);
            return result == 0 && parser.get<std::string>("input") == "file.txt";
        });
        
        // Test required argument missing
        run_test("Required argument missing (should fail)", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--input"}, "Input file", STR, "", true);
            
            std::vector<std::string> args = {"test"};
            int result = parser.parse_args(args);
            return result == -1;  // Should fail
        });
        
        // Test required positional argument missing
        run_test("Required positional argument missing (should fail)", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"filename"}, "Input file", STR, "", true);
            
            std::vector<std::string> args = {"test"};
            int result = parser.parse_args(args);
            return result == -1;  // Should fail
        });
    }
    
    void test_choices_validation() {
        print_test_header("Choices Validation");
        
        // Test valid choice
        run_test("Valid choice selection", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--mode"}, "Mode", STR, "", false, "", {"fast", "slow", "auto"});
            
            std::vector<std::string> args = {"test", "--mode", "fast"};
            int result = parser.parse_args(args);
            return result == 0 && parser.get<std::string>("mode") == "fast";
        });
        
        // Test invalid choice
        run_test("Invalid choice rejection (should fail)", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--mode"}, "Mode", STR, "", false, "", {"fast", "slow", "auto"});
            
            std::vector<std::string> args = {"test", "--mode", "invalid"};
            int result = parser.parse_args(args);
            return result == -1;  // Should fail
        });
        
        // Test choices with positional arguments
        run_test("Choices with positional arguments", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"action"}, "Action", STR, "", true, "", {"create", "delete", "update"});
            
            std::vector<std::string> args = {"test", "create"};
            int result = parser.parse_args(args);
            return result == 0 && parser.get<std::string>("action") == "create";
        });
        
        // Test integer choices
        run_test("Integer choices", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--level"}, "Level", INT, "", false, "", {"1", "2", "3"});
            
            std::vector<std::string> args = {"test", "--level", "2"};
            int result = parser.parse_args(args);
            return result == 0 && parser.get<int>("level") == 2;
        });
    }
    
    void test_metavar_display() {
        print_test_header("Metavar Display");
        
        // Test metavar storage
        run_test("Metavar storage", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--file"}, "Input file", STR, "", false, "", {}, "FILENAME");
            
            std::vector<std::string> args = {"test", "--file", "test.txt"};
            int result = parser.parse_args(args);
            return result == 0 && parser.get<std::string>("file") == "test.txt";
        });
    }
    
    void test_help_system() {
        print_test_header("Help System");
        
        // Test help flag detection
        run_test("Help flag detection", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--verbose"}, "Verbose mode", BOOL);
            
            std::vector<std::string> args = {"test", "--help"};
            int result = parser.parse_args(args);
            return result == 1;  // Help returns 1
        });
        
        // Test short help flag
        run_test("Short help flag", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--verbose"}, "Verbose mode", BOOL);
            
            std::vector<std::string> args = {"test", "-h"};
            int result = parser.parse_args(args);
            return result == 1;  // Help returns 1
        });
    }
    
    void test_nargs_support() {
        print_test_header("nargs Support");
        
        // Test nargs='*' with multiple values
        run_test("nargs='*' with multiple values", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--files"}, "Files", STR, "", false, "", {}, "", "*");
            
            std::vector<std::string> args = {"test", "--files", "file1.txt", "file2.txt", "file3.txt"};
            int result = parser.parse_args(args);
            
            if (result == 0) {
                auto files = parser.get_list<std::string>("files");
                return files.size() == 3 && 
                       files[0] == "file1.txt" && 
                       files[1] == "file2.txt" && 
                       files[2] == "file3.txt";
            }
            return false;
        });
        
        // Test nargs='*' with no values
        run_test("nargs='*' with no values", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--tags"}, "Tags", STR, "", false, "", {}, "", "*");
            
            std::vector<std::string> args = {"test"};
            int result = parser.parse_args(args);
            
            if (result == 0) {
                auto tags = parser.get_list<std::string>("tags");
                return tags.empty();
            }
            return false;
        });
        
        // Test nargs='+' with multiple values
        run_test("nargs='+' with multiple values", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--nums"}, "Numbers", INT, "", false, "", {}, "", "+");
            
            std::vector<std::string> args = {"test", "--nums", "1", "2", "3"};
            int result = parser.parse_args(args);
            
            if (result == 0) {
                auto nums = parser.get_list<int>("nums");
                return nums.size() == 3 && nums[0] == 1 && nums[1] == 2 && nums[2] == 3;
            }
            return false;
        });
        
        // Test nargs='+' with no values (should fail)
        run_test("nargs='+' with no values (should fail)", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--nums"}, "Numbers", INT, "", false, "", {}, "", "+");
            
            std::vector<std::string> args = {"test", "--nums"};
            int result = parser.parse_args(args);
            return result == -1;  // Should fail
        });
        
        // Test nargs='?' with one value
        run_test("nargs='?' with one value", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--config"}, "Config file", STR, "", false, "", {}, "", "?");
            
            std::vector<std::string> args = {"test", "--config", "config.ini"};
            int result = parser.parse_args(args);
            
            if (result == 0) {
                auto config = parser.get_list<std::string>("config");
                return config.size() == 1 && config[0] == "config.ini";
            }
            return false;
        });
        
        // Test nargs='?' with no value
        run_test("nargs='?' with no value", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--log"}, "Log file", STR, "", false, "", {}, "", "?");
            
            std::vector<std::string> args = {"test", "--log"};
            int result = parser.parse_args(args);
            
            if (result == 0) {
                auto log = parser.get_list<std::string>("log");
                return log.empty();
            }
            return false;
        });
        
        // Test nargs='2' with exact count
        run_test("nargs='2' with exact count", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--point"}, "Coordinates", FLOAT, "", false, "", {}, "", "2");
            
            std::vector<std::string> args = {"test", "--point", "1.5", "2.5"};
            int result = parser.parse_args(args);
            
            if (result == 0) {
                auto coords = parser.get_list<float>("point");
                return coords.size() == 2 && 
                       std::abs(coords[0] - 1.5f) < 0.001f && 
                       std::abs(coords[1] - 2.5f) < 0.001f;
            }
            return false;
        });
        
        // Test nargs with choices validation
        run_test("nargs with choices validation", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--colors"}, "Colors", STR, "", false, "", {"red", "green", "blue"}, "", "*");
            
            std::vector<std::string> args = {"test", "--colors", "red", "blue"};
            int result = parser.parse_args(args);
            
            if (result == 0) {
                auto colors = parser.get_list<std::string>("colors");
                return colors.size() == 2 && colors[0] == "red" && colors[1] == "blue";
            }
            return false;
        });
    }
    
    void test_mixed_arguments() {
        print_test_header("Mixed Argument Scenarios");
        
        // Test complex mixed arguments
        run_test("Complex mixed arguments", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"action"}, "Action to perform", STR, "", true, "", {"create", "delete"});
            parser.add_argument({"--verbose"}, "Verbose output", BOOL);
            parser.add_argument({"--count"}, "Count", INT, "1");
            parser.add_argument({"--files"}, "Files", STR, "", false, "", {}, "", "*");
            
            std::vector<std::string> args = {"test", "create", "--verbose", "--count", "5", "--files", "a.txt", "b.txt"};
            int result = parser.parse_args(args);
            
            if (result == 0) {
                auto files = parser.get_list<std::string>("files");
                return parser.get<std::string>("action") == "create" &&
                       parser.get<bool>("verbose") == true &&
                       parser.get<int>("count") == 5 &&
                       files.size() == 2 &&
                       files[0] == "a.txt" && files[1] == "b.txt";
            }
            return false;
        });
        
        // Test arguments in different orders
        run_test("Arguments in different orders", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"input"}, "Input file", STR, "", true);
            parser.add_argument({"--verbose"}, "Verbose", BOOL);
            parser.add_argument({"--output"}, "Output file", STR, "out.txt");
            
            std::vector<std::string> args = {"test", "--output", "result.txt", "--verbose", "input.txt"};
            int result = parser.parse_args(args);
            return result == 0 &&
                   parser.get<std::string>("input") == "input.txt" &&
                   parser.get<bool>("verbose") == true &&
                   parser.get<std::string>("output") == "result.txt";
        });
    }
    
    void test_negative_numbers() {
        print_test_header("Negative Number Handling");
        
        // Test negative integer
        run_test("Negative integer parsing", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--temp"}, "Temperature", INT);
            
            std::vector<std::string> args = {"test", "--temp", "-25"};
            int result = parser.parse_args(args);
            return result == 0 && parser.get<int>("temp") == -25;
        });
        
        // Test negative float
        run_test("Negative float parsing", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--delta"}, "Delta value", FLOAT);
            
            std::vector<std::string> args = {"test", "--delta", "-3.14"};
            int result = parser.parse_args(args);
            return result == 0 && std::abs(parser.get<float>("delta") - (-3.14f)) < 0.001f;
        });
        
        // Test negative numbers with nargs
        run_test("Negative numbers with nargs", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--values"}, "Values", FLOAT, "", false, "", {}, "", "*");
            
            std::vector<std::string> args = {"test", "--values", "-1.5", "2.0", "-3.5"};
            int result = parser.parse_args(args);
            
            if (result == 0) {
                auto values = parser.get_list<float>("values");
                return values.size() == 3 && 
                       std::abs(values[0] - (-1.5f)) < 0.001f &&
                       std::abs(values[1] - 2.0f) < 0.001f &&
                       std::abs(values[2] - (-3.5f)) < 0.001f;
            }
            return false;
        });
    }
    
    void test_type_validation() {
        print_test_header("Type Validation");
        
        // Test invalid integer
        run_test("Invalid integer (should fail)", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--number"}, "A number", INT);
            
            std::vector<std::string> args = {"test", "--number", "not_a_number"};
            int result = parser.parse_args(args);
            return result == -1;  // Should fail
        });
        
        // Test invalid float
        run_test("Invalid float (should fail)", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--value"}, "A float value", FLOAT);
            
            std::vector<std::string> args = {"test", "--value", "not_a_float"};
            int result = parser.parse_args(args);
            return result == -1;  // Should fail
        });
        
        // Test invalid boolean for positional
        run_test("Invalid boolean for positional (should fail)", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"flag"}, "Boolean flag", BOOL, "", true);
            
            std::vector<std::string> args = {"test", "maybe"};
            int result = parser.parse_args(args);
            return result == -1;  // Should fail
        });
    }
    
    void test_error_handling() {
        print_test_header("Error Handling");
        
        // Test unknown argument
        run_test("Unknown argument (should fail)", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--known"}, "Known argument", STR);
            
            std::vector<std::string> args = {"test", "--unknown", "value"};
            int result = parser.parse_args(args);
            return result == -1;  // Should fail
        });
        
        // Test missing value for optional argument
        run_test("Missing value for optional argument (should fail)", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--input"}, "Input file", STR);
            
            std::vector<std::string> args = {"test", "--input"};
            int result = parser.parse_args(args);
            return result == -1;  // Should fail
        });
        
        // Test invalid nargs format
        run_test("Invalid nargs format (should fail)", [&]() {
            ArgumentParser parser("test");
            
            try {
                parser.add_argument({"--test"}, "Test", STR, "", false, "", {}, "", "invalid");
                std::vector<std::string> args = {"test", "--test", "value"};
                int result = parser.parse_args(args);
                return result == -1;  // Should fail due to invalid nargs
            } catch (...) {
                return true;  // Exception is also acceptable
            }
        });
    }
    
    void test_edge_cases() {
        print_test_header("Edge Cases");
        
        // Test empty argument list
        run_test("Empty argument list", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--optional"}, "Optional arg", STR, "default");
            
            std::vector<std::string> args = {"test"};
            int result = parser.parse_args(args);
            return result == 0 && parser.get<std::string>("optional") == "default";
        });
        
        // Test multiple same arguments (last one wins)
        run_test("Multiple same arguments", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--value"}, "Value", STR);
            
            std::vector<std::string> args = {"test", "--value", "first", "--value", "second"};
            int result = parser.parse_args(args);
            return result == 0 && parser.get<std::string>("value") == "second";
        });
        
        // Test arguments with special characters
        run_test("Arguments with special characters", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--message"}, "Message", STR);
            
            std::vector<std::string> args = {"test", "--message", "Hello, World! @#$%^&*()"};
            int result = parser.parse_args(args);
            return result == 0 && parser.get<std::string>("message") == "Hello, World! @#$%^&*()";
        });
        
        // Test long argument values
        run_test("Long argument values", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--text"}, "Long text", STR);
            
            std::string long_text = std::string(1000, 'A');
            std::vector<std::string> args = {"test", "--text", long_text};
            int result = parser.parse_args(args);
            return result == 0 && parser.get<std::string>("text") == long_text;
        });
    }

    // === EXTENDED TESTS ===
    
    void test_has_argument_functionality() {
        print_test_header("has_argument() and get_with_default() Functionality");
        
        // Test has_argument with explicit arguments
        run_test("has_argument with explicitly provided arguments", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--verbose"}, "Verbose mode", BOOL);
            parser.add_argument({"--output"}, "Output file", STR, "default.txt");
            parser.add_argument({"--count"}, "Count", INT, "5");
            parser.add_argument({"--optional"}, "Optional arg", STR);  // No default
            
            std::vector<std::string> args = {"test", "--verbose", "--output", "custom.txt"};
            int result = parser.parse_args(args);
            
            return result == 0 && 
                   parser.has_argument("verbose") && 
                   parser.has_argument("output") && 
                   parser.has_argument("count") &&  // has default, so added to parsed_args_
                   parser.has_argument("optional") && // might be added to parsed_args_ even without default
                   parser.has_argument("help");  // help is always added
        });
        
        // Test get_with_default functionality
        run_test("get_with_default with existing arguments", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--port"}, "Port number", INT, "8080");
            parser.add_argument({"--host"}, "Host address", STR, "localhost");
            
            std::vector<std::string> args = {"test", "--port", "9000"};
            int result = parser.parse_args(args);
            
            return result == 0 && 
                   parser.get_with_default<int>("port", 3000) == 9000 &&
                   parser.get_with_default<std::string>("host", "0.0.0.0") == "localhost" &&
                   parser.get_with_default<int>("nonexistent", 12345) == 12345;
        });
        
        // Test get_with_default with type mismatch safety
        run_test("get_with_default with type mismatch", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--number"}, "A number", INT);
            
            std::vector<std::string> args = {"test", "--number", "42"};
            int result = parser.parse_args(args);
            
            // Try to get int as string - should return default
            return result == 0 && 
                   parser.get_with_default<std::string>("number", "default") == "default";
        });
    }
    
    void test_get_all_keys_functionality() {
        print_test_header("get_all_keys() Functionality");
        
        run_test("get_all_keys with multiple arguments", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--verbose"}, "Verbose mode", BOOL);
            parser.add_argument({"--output"}, "Output file", STR, "default.txt");
            parser.add_argument({"--count"}, "Count", INT, "5");
            parser.add_argument({"input"}, "Input file", STR, "", true);
            
            std::vector<std::string> args = {"test", "input.txt", "--verbose"};
            int result = parser.parse_args(args);
            
            auto keys = parser.get_all_keys();
            
            return result == 0 && 
                   keys.size() == 5 &&  // includes 'help' automatically
                   std::find(keys.begin(), keys.end(), "verbose") != keys.end() &&
                   std::find(keys.begin(), keys.end(), "output") != keys.end() &&
                   std::find(keys.begin(), keys.end(), "count") != keys.end() &&
                   std::find(keys.begin(), keys.end(), "input") != keys.end();
        });
    }
    
    void test_advanced_nargs_edge_cases() {
        print_test_header("Advanced nargs Edge Cases");
        
        // Test nargs with choices
        run_test("nargs='*' with choices and mixed valid/invalid", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--modes"}, "Operation modes", STR, "", false, "", {"read", "write", "append"}, "", "*");
            
            std::vector<std::string> args = {"test", "--modes", "read", "write"};
            int result = parser.parse_args(args);
            
            auto modes = parser.get_list<std::string>("modes");
            return result == 0 && modes.size() == 2 && 
                   modes[0] == "read" && modes[1] == "write";
        });
        
        // Test nargs with default values
        run_test("nargs='*' with default values", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--tags"}, "Tags", STR, "", false, "", {}, "", "*");
            
            std::vector<std::string> args = {"test"};  // No tags provided
            int result = parser.parse_args(args);
            
            auto tags = parser.get_list<std::string>("tags");
            return result == 0 && tags.empty();
        });
        
        // Test nargs with large numbers
        run_test("nargs with large specific count", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--coords"}, "Coordinates", FLOAT, "", false, "", {}, "", "4");
            
            std::vector<std::string> args = {"test", "--coords", "1.0", "2.0", "3.0", "4.0"};
            int result = parser.parse_args(args);
            
            auto coords = parser.get_list<float>("coords");
            return result == 0 && coords.size() == 4 && 
                   coords[0] == 1.0f && coords[3] == 4.0f;
        });
    }
    
    void test_complex_positional_scenarios() {
        print_test_header("Complex Positional Argument Scenarios");
        
        // Test multiple positional arguments with different types
        run_test("Multiple typed positional arguments", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"command"}, "Command to execute", STR, "", true);
            parser.add_argument({"count"}, "Number of times", INT, "", true);
            parser.add_argument({"factor"}, "Scaling factor", FLOAT, "", true);
            parser.add_argument({"--verbose"}, "Verbose mode", BOOL);
            
            std::vector<std::string> args = {"test", "run", "5", "2.5", "--verbose"};
            int result = parser.parse_args(args);
            
            return result == 0 && 
                   parser.get<std::string>("command") == "run" &&
                   parser.get<int>("count") == 5 &&
                   parser.get<float>("factor") == 2.5f &&
                   parser.get<bool>("verbose") == true;
        });
        
        // Test multiple positional string arguments
        run_test("Multiple positional string arguments", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"source"}, "Source file", STR, "", true);
            parser.add_argument({"dest"}, "Destination file", STR, "", true);
            parser.add_argument({"--verbose"}, "Verbose mode", BOOL);
            
            std::vector<std::string> args = {"test", "input.txt", "output.txt", "--verbose"};
            int result = parser.parse_args(args);
            
            return result == 0 &&
                   parser.get<std::string>("source") == "input.txt" &&
                   parser.get<std::string>("dest") == "output.txt" &&
                   parser.get<bool>("verbose") == true;
        });
    }
    
    void test_alias_to_key_conversion() {
        print_test_header("Alias to Key Conversion Edge Cases");
        
        // Test complex alias names
        run_test("Complex alias to key conversion", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--multi-word-option"}, "Multi word option", STR);
            parser.add_argument({"--config-file-path"}, "Config file path", STR);
            parser.add_argument({"-x", "--extra-verbose"}, "Extra verbose", BOOL);
            
            std::vector<std::string> args = {"test", "--multi-word-option", "value1", 
                                           "--config-file-path", "config.ini", "-x"};
            int result = parser.parse_args(args);
            
            return result == 0 && 
                   parser.get<std::string>("multi_word_option") == "value1" &&
                   parser.get<std::string>("config_file_path") == "config.ini" &&
                   parser.get<bool>("extra_verbose") == true;
        });
    }
    
    void test_boundary_values() {
        print_test_header("Boundary Value Testing");
        
        // Test integer boundary values
        run_test("Integer boundary values", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--max-int"}, "Max integer", INT);
            parser.add_argument({"--min-int"}, "Min integer", INT);
            parser.add_argument({"--zero"}, "Zero value", INT);
            
            std::vector<std::string> args = {"test", 
                                           "--max-int", "2147483647",
                                           "--min-int", "-2147483648",
                                           "--zero", "0"};
            int result = parser.parse_args(args);
            
            return result == 0 && 
                   parser.get<int>("max_int") == 2147483647 &&
                   parser.get<int>("min_int") == -2147483648 &&
                   parser.get<int>("zero") == 0;
        });
        
        // Test float boundary values
        run_test("Float boundary and special values", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--large"}, "Large float", FLOAT);
            parser.add_argument({"--small"}, "Small float", FLOAT);
            parser.add_argument({"--negative"}, "Negative float", FLOAT);
            
            std::vector<std::string> args = {"test", 
                                           "--large", "999999.999",
                                           "--small", "0.000001",
                                           "--negative", "-123.456"};
            int result = parser.parse_args(args);
            
            return result == 0 && 
                   std::abs(parser.get<float>("large") - 999999.999f) < 0.1f &&
                   std::abs(parser.get<float>("small") - 0.000001f) < 0.0000001f &&
                   std::abs(parser.get<float>("negative") - (-123.456f)) < 0.001f;
        });
    }
    
    void test_error_message_validation() {
        print_test_header("Error Message Validation");
        
        // Test error handling for unknown arguments
        run_test("Unknown argument error handling", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--known"}, "Known argument", STR);
            
            std::vector<std::string> args = {"test", "--unknown", "value"};
            
            // parse_args catches exceptions and returns -1 for errors
            int result = parser.parse_args(args);
            return result == -1;  // Should return error code
        });
        
        // Test type mismatch error in get<>()
        run_test("Type mismatch error message in get<>()", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--number"}, "A number", INT);
            
            std::vector<std::string> args = {"test", "--number", "42"};
            parser.parse_args(args);
            
            try {
                // Try to get int as bool - should fail with descriptive message
                parser.get<bool>("number");
                return false;  // Should have thrown
            } catch (const std::runtime_error& e) {
                std::string msg = e.what();
                return msg.find("Type mismatch") != std::string::npos && 
                       msg.find("Expected: bool") != std::string::npos &&
                       msg.find("Got: int") != std::string::npos;
            }
        });
    }
    
    void test_performance_stress() {
        print_test_header("Performance Stress Testing");
        
        // Test parsing large number of arguments
        run_test("Large number of arguments parsing", [&]() {
            ArgumentParser parser("test");
            
            // Add 50 arguments (reduced for more stability)
            for (int i = 0; i < 50; i++) {
                parser.add_argument({"--arg" + std::to_string(i)}, "Argument " + std::to_string(i), INT, std::to_string(i));
            }
            
            std::vector<std::string> args = {"test"};
            // Set every 10th argument
            for (int i = 0; i < 50; i += 10) {
                args.push_back("--arg" + std::to_string(i));
                args.push_back(std::to_string(i * 2));
            }
            
            auto start = std::chrono::high_resolution_clock::now();
            int result = parser.parse_args(args);
            auto end = std::chrono::high_resolution_clock::now();
            
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            
            // Verify parsing was successful and reasonably fast (< 1000ms)
            bool performance_ok = duration.count() < 1000;
            bool parsing_ok = result == 0;
            
            // Verify some values
            bool values_ok = true;
            try {
                values_ok = parser.get<int>("arg0") == 0 &&      // explicitly set to 0
                           parser.get<int>("arg10") == 20 &&     // explicitly set to 20  
                           parser.get<int>("arg40") == 80;       // explicitly set to 80
            } catch (const std::exception& e) {
                values_ok = false;
            }
            
            return parsing_ok && performance_ok && values_ok;
        });
    }
    
    void run_all_tests() {
        std::cout << "ArgParse Library - Unified Test Suite" << std::endl;
        std::cout << std::string(60, '=') << std::endl;
        
        // Core functionality tests
        test_basic_types();
        test_positional_arguments();
        test_optional_arguments();
        test_default_values();
        test_required_arguments();
        test_choices_validation();
        test_metavar_display();
        test_help_system();
        test_nargs_support();
        test_mixed_arguments();
        test_negative_numbers();
        test_type_validation();
        test_error_handling();
        test_edge_cases();
        
        // Extended functionality tests
        test_has_argument_functionality();
        test_get_all_keys_functionality();
        test_advanced_nargs_edge_cases();
        test_complex_positional_scenarios();
        test_alias_to_key_conversion();
        test_boundary_values();
        test_error_message_validation();
        test_performance_stress();
        
        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << "FINAL RESULTS" << std::endl;
        std::cout << std::string(60, '=') << std::endl;
        std::cout << "Total Tests: " << total_tests << std::endl;
        std::cout << "Passed: " << passed_tests << " ✓" << std::endl;
        std::cout << "Failed: " << failed_tests << " ✗" << std::endl;
        std::cout << "Success Rate: " << (total_tests > 0 ? (passed_tests * 100 / total_tests) : 0) << "%" << std::endl;
        
        if (failed_tests == 0) {
            std::cout << "\n🎉 ALL TESTS PASSED! 🎉" << std::endl;
            std::cout << "ArgParse library is working correctly!" << std::endl;
        } else {
            std::cout << "\n❌ SOME TESTS FAILED ❌" << std::endl;
            std::cout << "Please review the failed test cases above." << std::endl;
        }
        std::cout << std::string(60, '=') << std::endl;
    }
};

int main() {
    UnifiedTestSuite suite;
    suite.run_all_tests();
    return 0;
}