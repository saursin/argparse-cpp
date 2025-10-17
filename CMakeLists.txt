cmake_minimum_required(VERSION 3.12)
project(argparse-cpp VERSION 1.0.0 LANGUAGES CXX)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Library configuration
set(ARGPARSE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(ARGPARSE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Create library
add_library(argparse 
    ${ARGPARSE_SOURCE_DIR}/argparse.cpp
)

# Set include directories
target_include_directories(argparse PUBLIC 
    $<BUILD_INTERFACE:${ARGPARSE_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# Set compiler features
target_compile_features(argparse PUBLIC cxx_std_17)

# Optional: Build tests
option(ARGPARSE_BUILD_TESTS "Build tests" ON)
if(ARGPARSE_BUILD_TESTS)
    add_executable(comprehensive_test tests/comprehensive_test.cpp)
    target_link_libraries(comprehensive_test argparse)
    target_include_directories(comprehensive_test PRIVATE ${ARGPARSE_INCLUDE_DIR})
    
    # Add test
    enable_testing()
    add_test(NAME comprehensive_test COMMAND comprehensive_test)
endif()

# Optional: Build examples
option(ARGPARSE_BUILD_EXAMPLES "Build examples" ON)
if(ARGPARSE_BUILD_EXAMPLES)
    file(GLOB EXAMPLE_SOURCES examples/*.cpp)
    foreach(EXAMPLE_SOURCE ${EXAMPLE_SOURCES})
        get_filename_component(EXAMPLE_NAME ${EXAMPLE_SOURCE} NAME_WE)
        add_executable(${EXAMPLE_NAME} ${EXAMPLE_SOURCE})
        target_link_libraries(${EXAMPLE_NAME} argparse)
        target_include_directories(${EXAMPLE_NAME} PRIVATE ${ARGPARSE_INCLUDE_DIR})
    endforeach()
endif()

# Installation
include(GNUInstallDirs)

install(TARGETS argparse
    EXPORT argparse-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(FILES include/argparse.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT argparse-targets
    FILE argparse-config.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/argparse
)

# Package configuration
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/argparse-config-version.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)