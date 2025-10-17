#include <iostream>
#include "argparse.h"

using namespace ArgParse;

int main(int argc, char* argv[]) {
    // Create argument parser
    ArgumentParser parser("example");
    
    // Add arguments
    parser.add_argument({"input"}, "Input file", STR, "", true);
    parser.add_argument({"-v", "--verbose"}, "Enable verbose output", BOOL);
    parser.add_argument({"--count"}, "Number of items to process", INT, "10");
    parser.add_argument({"--format"}, "Output format", STR, "json", false, "", {"json", "xml", "csv"});
    parser.add_argument({"--files"}, "Additional files", STR, "", false, "", {}, "", "*");
    
    // Parse command line arguments
    if (parser.parse_args(argc, argv) != 0) {
        return 1;  // Parsing failed
    }
    
    // Extract values
    std::string input = parser.get<std::string>("input");
    bool verbose = parser.get<bool>("verbose");
    int count = parser.get<int>("count");
    std::string format = parser.get<std::string>("format");
    auto files = parser.get_list<std::string>("files");
    
    // Use the values
    std::cout << "Processing: " << input << std::endl;
    std::cout << "Format: " << format << std::endl;
    std::cout << "Count: " << count << std::endl;
    
    if (verbose) {
        std::cout << "Verbose mode enabled" << std::endl;
    }
    
    if (!files.empty()) {
        std::cout << "Additional files: ";
        for (const auto& file : files) {
            std::cout << file << " ";
        }
        std::cout << std::endl;
    }
    
    return 0;
}