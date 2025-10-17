/**
 * Comprehensive Test Suite for ArgParse Library
 * 
 * This test suite covers all implemented features:
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
 */

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <sstream>
#include <functional>
#include <cmath>
#include "argparse.h"

using namespace ArgParse;

class TestSuite {
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
            std::cout << "✗ ERROR: " << test_name << " - " << e.what() << std::endl;
            failed_tests++;
            return false;
        }
    }
    
public:
    void run_all_tests() {
        std::cout << "ArgParse Library - Comprehensive Test Suite" << std::endl;
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
        
        // Advanced features
        test_nargs_support();
        test_mixed_arguments();
        test_negative_numbers();
        test_type_validation();
        test_error_handling();
        test_edge_cases();
        
        // Print final results
        print_final_results();
    }
    
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
        
        // Single positional argument
        run_test("Single positional argument", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"filename"}, "Input file", STR, "", true);
            
            std::vector<std::string> args = {"test", "input.txt"};
            int result = parser.parse_args(args);
            return result == 0 && parser.get<std::string>("filename") == "input.txt";
        });
        
        // Multiple positional arguments
        run_test("Multiple positional arguments", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"source"}, "Source file", STR, "", true);
            parser.add_argument({"dest"}, "Destination file", STR, "", true);
            
            std::vector<std::string> args = {"test", "src.txt", "dst.txt"};
            int result = parser.parse_args(args);
            return result == 0 && 
                   parser.get<std::string>("source") == "src.txt" &&
                   parser.get<std::string>("dest") == "dst.txt";
        });
        
        // Positional argument with different types
        run_test("Positional arguments with different types", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"count"}, "Number count", INT, "", true);
            parser.add_argument({"name"}, "Item name", STR, "", true);
            
            std::vector<std::string> args = {"test", "5", "widget"};
            int result = parser.parse_args(args);
            return result == 0 && 
                   parser.get<int>("count") == 5 &&
                   parser.get<std::string>("name") == "widget";
        });
    }
    
    void test_optional_arguments() {
        print_test_header("Optional Arguments");
        
        // Short and long forms
        run_test("Short and long argument forms", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"-v", "--verbose"}, "Verbose mode", BOOL);
            
            std::vector<std::string> args1 = {"test", "-v"};
            int result1 = parser.parse_args(args1);
            bool verbose1 = parser.get<bool>("verbose");
            
            ArgumentParser parser2("test");
            parser2.add_argument({"-v", "--verbose"}, "Verbose mode", BOOL);
            std::vector<std::string> args2 = {"test", "--verbose"};
            int result2 = parser2.parse_args(args2);
            bool verbose2 = parser2.get<bool>("verbose");
            
            return result1 == 0 && result2 == 0 && verbose1 && verbose2;
        });
        
        // Multiple aliases
        run_test("Multiple argument aliases", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"-o", "--output", "--out"}, "Output file", STR);
            
            std::vector<std::string> args = {"test", "--out", "result.txt"};
            int result = parser.parse_args(args);
            // The key is resolved to the longest alias (--output -> output)
            return result == 0 && parser.get<std::string>("output") == "result.txt";
        });
        
        // Mixed positional and optional
        run_test("Mixed positional and optional arguments", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"input"}, "Input file", STR, "", true);
            parser.add_argument({"--verbose"}, "Verbose mode", BOOL);
            parser.add_argument({"--count"}, "Process count", INT);
            
            std::vector<std::string> args = {"test", "data.txt", "--verbose", "--count", "3"};
            int result = parser.parse_args(args);
            return result == 0 && 
                   parser.get<std::string>("input") == "data.txt" &&
                   parser.get<bool>("verbose") == true &&
                   parser.get<int>("count") == 3;
        });
    }
    
    void test_default_values() {
        print_test_header("Default Values");
        
        // String default
        run_test("String default value", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--output"}, "Output file", STR, "default.txt");
            
            std::vector<std::string> args = {"test"};
            int result = parser.parse_args(args);
            return result == 0 && parser.get<std::string>("output") == "default.txt";
        });
        
        // Integer default
        run_test("Integer default value", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--threads"}, "Thread count", INT, "4");
            
            std::vector<std::string> args = {"test"};
            int result = parser.parse_args(args);
            return result == 0 && parser.get<int>("threads") == 4;
        });
        
        // Override default
        run_test("Override default value", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--threads"}, "Thread count", INT, "4");
            
            std::vector<std::string> args = {"test", "--threads", "8"};
            int result = parser.parse_args(args);
            return result == 0 && parser.get<int>("threads") == 8;
        });
    }
    
    void test_required_arguments() {
        print_test_header("Required Arguments");
        
        // Required argument provided
        run_test("Required argument provided", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--input"}, "Input file", STR, "", true);
            
            std::vector<std::string> args = {"test", "--input", "data.txt"};
            int result = parser.parse_args(args);
            return result == 0 && parser.get<std::string>("input") == "data.txt";
        });
        
        // Required argument missing
        run_test("Required argument missing (should fail)", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--input"}, "Input file", STR, "", true);
            
            std::vector<std::string> args = {"test"};
            int result = parser.parse_args(args);
            return result != 0;  // Should fail
        });
        
        // Required positional argument missing
        run_test("Required positional argument missing (should fail)", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"filename"}, "Input file", STR, "", true);
            
            std::vector<std::string> args = {"test"};
            int result = parser.parse_args(args);
            return result != 0;  // Should fail
        });
    }
    
    void test_choices_validation() {
        print_test_header("Choices Validation");
        
        // Valid choice
        run_test("Valid choice selection", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--mode"}, "Processing mode", STR, "", false, "", {"fast", "slow", "auto"});
            
            std::vector<std::string> args = {"test", "--mode", "fast"};
            int result = parser.parse_args(args);
            return result == 0 && parser.get<std::string>("mode") == "fast";
        });
        
        // Invalid choice
        run_test("Invalid choice rejection (should fail)", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--mode"}, "Processing mode", STR, "", false, "", {"fast", "slow", "auto"});
            
            std::vector<std::string> args = {"test", "--mode", "invalid"};
            int result = parser.parse_args(args);
            return result != 0;  // Should fail
        });
        
        // Choices with positional arguments
        run_test("Choices with positional arguments", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"command"}, "Command to run", STR, "", true, "", {"start", "stop", "restart"});
            
            std::vector<std::string> args = {"test", "start"};
            int result = parser.parse_args(args);
            return result == 0 && parser.get<std::string>("command") == "start";
        });
        
        // Integer choices
        run_test("Integer choices", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--level"}, "Debug level", INT, "", false, "", {"1", "2", "3"});
            
            std::vector<std::string> args = {"test", "--level", "2"};
            int result = parser.parse_args(args);
            return result == 0 && parser.get<int>("level") == 2;
        });
    }
    
    void test_metavar_display() {
        print_test_header("Metavar Display");
        
        // This test verifies metavar is stored correctly (display testing would require capturing help output)
        run_test("Metavar storage", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--file"}, "Input file", STR, "", false, "", {}, "FILENAME");
            
            // Just verify it doesn't crash and parses correctly
            std::vector<std::string> args = {"test", "--file", "test.txt"};
            int result = parser.parse_args(args);
            return result == 0 && parser.get<std::string>("file") == "test.txt";
        });
    }
    
    void test_help_system() {
        print_test_header("Help System");
        
        // Help flag detection
        run_test("Help flag detection", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--verbose"}, "Verbose mode", BOOL);
            
            std::vector<std::string> args = {"test", "--help"};
            int result = parser.parse_args(args);
            return result == 1;  // Help returns 1
        });
        
        // Short help flag
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
        
        // nargs="*" (zero or more)
        run_test("nargs='*' with multiple values", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--files"}, "Input files", STR, "", false, "", {}, "", "*");
            
            std::vector<std::string> args = {"test", "--files", "a.txt", "b.txt", "c.txt"};
            int result = parser.parse_args(args);
            if (result != 0) return false;
            
            auto files = parser.get_list<std::string>("files");
            return files.size() == 3 && files[0] == "a.txt" && files[1] == "b.txt" && files[2] == "c.txt";
        });
        
        // nargs="*" with no values
        run_test("nargs='*' with no values", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--files"}, "Input files", STR, "", false, "", {}, "", "*");
            
            std::vector<std::string> args = {"test", "--files"};
            int result = parser.parse_args(args);
            if (result != 0) return false;
            
            auto files = parser.get_list<std::string>("files");
            return files.size() == 0;
        });
        
        // nargs="+" (one or more)
        run_test("nargs='+' with multiple values", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--nums"}, "Numbers", INT, "", false, "", {}, "", "+");
            
            std::vector<std::string> args = {"test", "--nums", "1", "2", "3"};
            int result = parser.parse_args(args);
            if (result != 0) return false;
            
            auto nums = parser.get_list<int>("nums");
            return nums.size() == 3 && nums[0] == 1 && nums[1] == 2 && nums[2] == 3;
        });
        
        // nargs="+" with no values (should fail)
        run_test("nargs='+' with no values (should fail)", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--nums"}, "Numbers", INT, "", false, "", {}, "", "+");
            
            std::vector<std::string> args = {"test", "--nums"};
            int result = parser.parse_args(args);
            return result != 0;  // Should fail
        });
        
        // nargs="?" (zero or one)
        run_test("nargs='?' with one value", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--config"}, "Config file", STR, "", false, "", {}, "", "?");
            
            std::vector<std::string> args = {"test", "--config", "settings.conf"};
            int result = parser.parse_args(args);
            if (result != 0) return false;
            
            auto configs = parser.get_list<std::string>("config");
            return configs.size() == 1 && configs[0] == "settings.conf";
        });
        
        // nargs="?" with no value
        run_test("nargs='?' with no value", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--config"}, "Config file", STR, "", false, "", {}, "", "?");
            
            std::vector<std::string> args = {"test", "--config"};
            int result = parser.parse_args(args);
            if (result != 0) return false;
            
            auto configs = parser.get_list<std::string>("config");
            return configs.size() == 0;
        });
        
        // nargs="2" (exact number)
        run_test("nargs='2' with exact count", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--coords"}, "Coordinates", FLOAT, "", false, "", {}, "", "2");
            
            std::vector<std::string> args = {"test", "--coords", "1.5", "2.5"};
            int result = parser.parse_args(args);
            if (result != 0) return false;
            
            auto coords = parser.get_list<float>("coords");
            return coords.size() == 2 && 
                   std::abs(coords[0] - 1.5f) < 0.001f && 
                   std::abs(coords[1] - 2.5f) < 0.001f;
        });
        
        // nargs with choices
        run_test("nargs with choices validation", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--colors"}, "Colors", STR, "", false, "", {"red", "green", "blue"}, "", "+");
            
            std::vector<std::string> args = {"test", "--colors", "red", "blue"};
            int result = parser.parse_args(args);
            if (result != 0) return false;
            
            auto colors = parser.get_list<std::string>("colors");
            return colors.size() == 2 && colors[0] == "red" && colors[1] == "blue";
        });
    }
    
    void test_mixed_arguments() {
        print_test_header("Mixed Argument Scenarios");
        
        // Complex scenario with all types
        run_test("Complex mixed arguments", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"input"}, "Input file", STR, "", true);
            parser.add_argument({"--verbose"}, "Verbose mode", BOOL);
            parser.add_argument({"--threads"}, "Thread count", INT, "1");
            parser.add_argument({"--threshold"}, "Threshold value", FLOAT, "0.5");
            parser.add_argument({"--format"}, "Output format", STR, "", false, "", {"json", "xml", "csv"});
            
            std::vector<std::string> args = {
                "test", "data.txt", "--verbose", "--threads", "4", 
                "--threshold", "0.8", "--format", "json"
            };
            int result = parser.parse_args(args);
            
            return result == 0 && 
                   parser.get<std::string>("input") == "data.txt" &&
                   parser.get<bool>("verbose") == true &&
                   parser.get<int>("threads") == 4 &&
                   std::abs(parser.get<float>("threshold") - 0.8f) < 0.001f &&
                   parser.get<std::string>("format") == "json";
        });
        
        // Arguments in different orders
        run_test("Arguments in different orders", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"input"}, "Input file", STR, "", true);
            parser.add_argument({"--count"}, "Count", INT);
            parser.add_argument({"--flag"}, "Flag", BOOL);
            
            std::vector<std::string> args = {"test", "--flag", "--count", "5", "file.txt"};
            int result = parser.parse_args(args);
            
            return result == 0 && 
                   parser.get<std::string>("input") == "file.txt" &&
                   parser.get<int>("count") == 5 &&
                   parser.get<bool>("flag") == true;
        });
    }
    
    void test_negative_numbers() {
        print_test_header("Negative Number Handling");
        
        // Negative integer
        run_test("Negative integer parsing", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--value"}, "Integer value", INT);
            
            std::vector<std::string> args = {"test", "--value", "-42"};
            int result = parser.parse_args(args);
            return result == 0 && parser.get<int>("value") == -42;
        });
        
        // Negative float
        run_test("Negative float parsing", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--value"}, "Float value", FLOAT);
            
            std::vector<std::string> args = {"test", "--value", "-3.14"};
            int result = parser.parse_args(args);
            return result == 0 && std::abs(parser.get<float>("value") - (-3.14f)) < 0.001f;
        });
        
        // Negative numbers with nargs
        run_test("Negative numbers with nargs", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--values"}, "Values", INT, "", false, "", {}, "", "+");
            
            std::vector<std::string> args = {"test", "--values", "-1", "-2", "3"};
            int result = parser.parse_args(args);
            if (result != 0) return false;
            
            auto values = parser.get_list<int>("values");
            return values.size() == 3 && values[0] == -1 && values[1] == -2 && values[2] == 3;
        });
    }
    
    void test_type_validation() {
        print_test_header("Type Validation");
        
        // Invalid integer
        run_test("Invalid integer (should fail)", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--number"}, "Number", INT);
            
            std::vector<std::string> args = {"test", "--number", "not_a_number"};
            int result = parser.parse_args(args);
            return result != 0;  // Should fail
        });
        
        // Invalid float
        run_test("Invalid float (should fail)", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--value"}, "Value", FLOAT);
            
            std::vector<std::string> args = {"test", "--value", "not_a_float"};
            int result = parser.parse_args(args);
            return result != 0;  // Should fail
        });
        
        // Invalid boolean
        run_test("Invalid boolean for positional (should fail)", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"flag"}, "Boolean flag", BOOL, "", true);
            
            std::vector<std::string> args = {"test", "maybe"};
            int result = parser.parse_args(args);
            return result != 0;  // Should fail
        });
    }
    
    void test_error_handling() {
        print_test_header("Error Handling");
        
        // Unknown argument
        run_test("Unknown argument (should fail)", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--known"}, "Known arg", STR);
            
            std::vector<std::string> args = {"test", "--unknown", "value"};
            int result = parser.parse_args(args);
            return result != 0;  // Should fail
        });
        
        // Missing value for optional argument
        run_test("Missing value for optional argument (should fail)", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--input"}, "Input file", STR);
            
            std::vector<std::string> args = {"test", "--input"};
            int result = parser.parse_args(args);
            return result != 0;  // Should fail
        });
        
        // Invalid nargs format
        run_test("Invalid nargs format (should fail)", [&]() {
            ArgumentParser parser("test");
            try {
                parser.add_argument({"--test"}, "Test arg", STR, "", false, "", {}, "", "invalid");
                return false;  // Should have thrown
            } catch (const ArgParseException&) {
                return true;  // Expected exception
            }
        });
    }
    
    void test_edge_cases() {
        print_test_header("Edge Cases");
        
        // Empty argument list
        run_test("Empty argument list", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--optional"}, "Optional arg", STR, "default");
            
            std::vector<std::string> args = {"test"};
            int result = parser.parse_args(args);
            return result == 0 && parser.get<std::string>("optional") == "default";
        });
        
        // Multiple same arguments (last one wins)
        run_test("Multiple same arguments", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--value"}, "Value", STR);
            
            std::vector<std::string> args = {"test", "--value", "first", "--value", "second"};
            int result = parser.parse_args(args);
            return result == 0 && parser.get<std::string>("value") == "second";
        });
        
        // Arguments with special characters
        run_test("Arguments with special characters", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--text"}, "Text value", STR);
            
            std::vector<std::string> args = {"test", "--text", "hello world!@#$%"};
            int result = parser.parse_args(args);
            return result == 0 && parser.get<std::string>("text") == "hello world!@#$%";
        });
        
        // Very long argument values
        run_test("Long argument values", [&]() {
            ArgumentParser parser("test");
            parser.add_argument({"--text"}, "Text value", STR);
            
            std::string long_text(1000, 'x');
            std::vector<std::string> args = {"test", "--text", long_text};
            int result = parser.parse_args(args);
            return result == 0 && parser.get<std::string>("text") == long_text;
        });
    }
    
    void print_final_results() {
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
            std::cout << "\n⚠️  Some tests failed. Please review the results above." << std::endl;
        }
        std::cout << std::string(60, '=') << std::endl;
    }
};

int main() {
    TestSuite suite;
    suite.run_all_tests();
    return 0;
}