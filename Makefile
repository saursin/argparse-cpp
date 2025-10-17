# ArgParse C++ Library Makefile

CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -O2
INCLUDES = -Iinclude
SRCDIR = src
OBJDIR = build/obj
LIBDIR = build/lib
TESTDIR = tests
EXAMPLEDIR = examples

PREFIX? = /usr/local

# Library name
LIBNAME = argparse
STATIC_LIB = $(LIBDIR)/lib$(LIBNAME).a
SHARED_LIB = $(LIBDIR)/lib$(LIBNAME).so

# Source files
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

# Test files
TEST_SOURCES = $(wildcard $(TESTDIR)/*.cpp)
TEST_TARGETS = $(TEST_SOURCES:$(TESTDIR)/%.cpp=build/%)

# Example files
EXAMPLE_SOURCES = $(wildcard $(EXAMPLEDIR)/*.cpp)
EXAMPLE_TARGETS = $(EXAMPLE_SOURCES:$(EXAMPLEDIR)/%.cpp=build/%)

.PHONY: all clean static shared tests examples install

# Default target
all: static

# Create directories
$(OBJDIR) $(LIBDIR):
	mkdir -p $@

# Compile object files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Build static library
static: $(STATIC_LIB)

$(STATIC_LIB): $(OBJECTS) | $(LIBDIR)
	ar rcs $@ $^
	@echo "Static library built: $@"

# Build shared library
shared: $(SHARED_LIB)

$(SHARED_LIB): $(OBJECTS) | $(LIBDIR)
	$(CXX) -shared -o $@ $^
	@echo "Shared library built: $@"

# Build tests
tests: static $(TEST_TARGETS)

build/test_%: $(TESTDIR)/test_%.cpp $(STATIC_LIB)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< -L$(LIBDIR) -l$(LIBNAME) -o $@

# Special target for comprehensive_test
build/comprehensive_test: $(TESTDIR)/comprehensive_test.cpp $(STATIC_LIB)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< -L$(LIBDIR) -l$(LIBNAME) -o $@

# Special target for extended_test
build/extended_test: $(TESTDIR)/extended_test.cpp $(STATIC_LIB)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< -L$(LIBDIR) -l$(LIBNAME) -o $@

# Special target for unified_test (all tests in one file)
build/unified_test: $(TESTDIR)/unified_test.cpp $(STATIC_LIB)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< -L$(LIBDIR) -l$(LIBNAME) -o $@

# Build examples
examples: static $(EXAMPLE_TARGETS)

build/%: $(EXAMPLEDIR)/%.cpp $(STATIC_LIB)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< -L$(LIBDIR) -l$(LIBNAME) -o $@

# Run tests
test: build/comprehensive_test
	@echo "Running comprehensive tests..."
	./build/comprehensive_test

# Clean build artifacts
clean:
	rm -rf build/

# Install library (requires sudo)
install: static
	cp include/argparse.h $(PREFIX)/include/
	cp $(STATIC_LIB) $(PREFIX)/lib/
	ldconfig
	@echo "ArgParse library installed to $(PREFIX)"

# Show help
help:
	@echo "Available targets:"
	@echo "  all           - Build static library (default)"
	@echo "  static        - Build static library"
	@echo "  shared        - Build shared library"
	@echo "  tests         - Build and compile tests"  
	@echo "  examples      - Build example programs"
	@echo "  test          - Run comprehensive test suite"
	@echo "  test-extended - Run extended test suite"
	@echo "  test-unified  - Run unified test suite (all tests in one file)"
	@echo "  test-all      - Run both comprehensive and extended tests"
	@echo "  clean         - Remove build artifacts"
	@echo "  install       - Install library system-wide (requires sudo)"
	@echo "  help          - Show this help message"